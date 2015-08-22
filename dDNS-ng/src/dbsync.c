#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "common.h"
#include "dynsrv.h"
#include "dbsync.h"

static int writeFile(char * path, domain_t * data, int max);
static MYSQL_RES * queryDomains(MYSQL * dbh, int logger);
static MYSQL_RES * querySubDomain(MYSQL * dbh, char * domain, int server_log);
static int fileExist(char * path);
static int updateNamedConf(char * path, char * named_conf_path, char * domain, int logger);
static void clearData(domain_t * data, int max);

int dbsync(config_t * cfg, int server_log) {
	MYSQL * dbh;
	MYSQL_RES * domains;
	MYSQL_RES * nsrec;
	MYSQL_ROW row, row2;
	domain_t * data;
	char * path_prefix = cfg->server.zonedir;
	char * path;
	char * zoneName;
	int recCnt;
	int i;
	pid_t reload_pid;

	log_event(server_log, " INFO: synchronization process is ready\n", NULL);
	while(1) {
		dbh = dbLogin(cfg);
		if(dbh == NULL) {
			log_event(server_log, " ERROR Error connecting to database\n", NULL);
			exit(-1);
		}
		if((domains = queryDomains(dbh, server_log)) == NULL) {
			mysql_close(dbh);
			exit(-1);
		}
		while((row = mysql_fetch_row(domains)) != NULL) {
			data = (domain_t *) malloc(sizeof(domain_t));
			i = 0;

			data->origin = (char *) malloc((strlen(row[1]) + 1) * sizeof(char));
			strcpy(data->origin, row[1]);
			data->ttl = atoi(row[2]);
			data->master_dns = (char *) malloc((strlen(row[4]) + 1) * sizeof(char));
			strcpy(data->master_dns, row[4]);
			data->admin_contact = (char *) malloc((strlen(row[3]) + 1) * sizeof(char));
			strcpy(data->admin_contact, row[3]);
			data->serial = atoi(row[5]);
			data->expiry = atoi(row[8]);
			data->maximum = atoi(row[9]);
			data->refresh = atoi(row[6]);
			data->retry = atoi(row[7]);

			path = (char *) malloc((strlen(path_prefix) + strlen(data->origin) + 1) * sizeof(char));
			strcpy(path, path_prefix);
			strncat(path, data->origin, strlen(data->origin) - 1);
			path[strlen(path)] = '\0';

			zoneName = (char *) malloc((strlen(data->origin) + 1) * sizeof(char));
			bzero(zoneName, (strlen(data->origin) + 1) * sizeof(char));
			strcpy(zoneName, data->origin);
			zoneName[strlen(zoneName)-1] = '\0';

			if((nsrec = querySubDomain(dbh, data->origin, server_log)) == NULL) {
				mysql_close(dbh);
				exit(-1);
			}
			recCnt = mysql_num_rows(nsrec);
			data->records = (struct subdomain **) malloc(sizeof(struct subdomain) * recCnt);
			while((row2 = mysql_fetch_row(nsrec)) != NULL) {
				data->records[i] = (struct subdomain *) malloc(sizeof(char *) * 3);
				data->records[i]->subd_name = (char *) malloc((strlen(row2[0]) + 1) * sizeof(char));
				strcpy(data->records[i]->subd_name, row2[0]);
				data->records[i]->ip = (char *) malloc((strlen(row2[1]) + 1) * sizeof(char));
				strcpy(data->records[i]->ip, row2[1]);
				data->records[i]->type = (char *) malloc((strlen(row2[2]) + 1) * sizeof(char));
				strcpy(data->records[i]->type, row2[2]);
				i++;
			}
			if(fileExist(path) == 0) {
				if(writeFile(path, data, recCnt) < 0) {
					log_event(server_log, " ERROR Create zone: ", path, " failed\n", NULL);
					break;
				}
				if(updateNamedConf(path, cfg->server.namedconf, zoneName, server_log) == 0)
					log_event(server_log, " ERROR FileUpdate: ",cfg->server.namedconf, " failed\n", NULL);
				else {
					reload_pid = fork();
					if(reload_pid == 0)
						namedReload();
				    else if(reload_pid > 0)
				    	waitpid(reload_pid, NULL, WNOHANG);
				    else
				    	log_event(server_log, " ERROR Reload named failed\n", NULL);
				}
			}
			mysql_free_result(nsrec);
			free(zoneName);
			free(path);
			clearData(data, i);
		}
		mysql_free_result(domains);
		mysql_close(dbh);
		sleep(15);
	}
	free(cfg->server.namedconf);
	return 1;
}
static int writeFile(char * path, domain_t * data, int max) {
	FILE * zone;
	int i;

	zone = fopen(path, "w");
	if(zone == NULL)
		return -1;
	fprintf(zone, "$TTL %d\n", data->ttl);
	fprintf(zone, "$ORIGIN %s\n", data->origin);
	fprintf(zone, "@\tIN\tSOA\t%s %s (\n", data->master_dns, data->admin_contact);
	fprintf(zone, "\t\t%d ; serial\n", data->serial);
	fprintf(zone, "\t\t%d ; refresh\n", data->refresh);
	fprintf(zone, "\t\t%d ; retry\n", data->retry);
	fprintf(zone, "\t\t%d ; expire\n", data->expiry);
	fprintf(zone, "\t\t%d ; maximum\n\t\t)\n", data->maximum);
	for(i = 0; i < max; i++)
		fprintf(zone, "%s\tIN\t\%s\t%s\n",
				data->records[i]->subd_name, data->records[i]->type, data->records[i]->ip);
	fclose(zone);
	free(path);

	return 1;
}
static MYSQL_RES * queryDomains(MYSQL * dbh, int logger) {
	MYSQL_RES * res;

	char * query = "SELECT id,domain,ttl,admin_contact,master_dns,serial,refresh,retry,expiry,maximum, owner \
				FROM domains WHERE owner NOT LIKE 'root'";

	if(mysql_query(dbh, query) != 0) {
		log_event(logger, " ERROR SQL query ", query, " failed\n", NULL);
		return NULL;
	}
	if((res = mysql_store_result(dbh)) == NULL) {
		log_event(logger, " ERROR Empty SQL query result\n", NULL);
		return NULL;
	}
	return res;
}
static MYSQL_RES * querySubDomain(MYSQL * dbh, char * domain, int logger) {
	MYSQL_RES * res;
	char * query_prefix = "SELECT subdomain,ip,type FROM subdomains \
			WHERE domain_id = (SELECT id FROM domains WHERE domain = '";
	char * query;
	size_t memsize = (strlen(query_prefix) + strlen(domain) + 5) * sizeof(char);

	query = (char *) malloc(memsize);
	strcpy(query, query_prefix);
	strcat(query, domain);
	strcat(query, "')");
	if(mysql_query(dbh, query) != 0) {
		log_event(logger, " ERROR SQL query ", query, " failed\n", NULL);
		return NULL;
	}
	if((res = mysql_store_result(dbh)) == NULL) {
		log_event(logger, " ERROR Empty SQL query result\n", NULL);
		return NULL;
	}
	free(query);

	return res;
}
static int fileExist(char * path) {
	FILE * f;

	if((f = fopen(path, "r")) == NULL)
		return 0;
	else {
		fclose(f);
		return 1;
	}
}
static int updateNamedConf(char * path, char * named_conf_path, char * domain, int logger) {
	FILE * named_conf;

	if((named_conf = fopen(named_conf_path, "a")) == NULL) {
		log_event(logger, " ERROR opening ", path, "\n");
		return 0;
	}

	fprintf(named_conf, "zone \"%s\" {\n", domain);
	fprintf(named_conf, "\ttype master;\n");
	fprintf(named_conf, "\tallow-update { none; };\n");
	fprintf(named_conf, "\tfile \"%s\";\n", path);
	fprintf(named_conf, "};\n");

	fclose(named_conf);
	return 1;
}
static void clearData(domain_t * data, int max) {
	int i;

	for(i = 0; i < max; i++) {
		free(data->records[i]->ip);
		free(data->records[i]->subd_name);
		free(data->records[i]->type);
		free(data->records[i]);
	}
	free(data->admin_contact);
	free(data->master_dns);
	free(data->origin);
	free(data->records);
	free(data);
}
