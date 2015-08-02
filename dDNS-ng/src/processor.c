#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "dynsrv.h"
#include "clientmanager.h"
#include "auth.h"

static void clearConnData(REMOTEDATA_t * conn);
static void clearDBData(DB_USERDATA_t * db);

int clientManager(config_t * cfg_file, int logfd, int sockfd) {
	MYSQL * dbh;
	MYSQL_RES * res;
	MYSQL_ROW row;

	REMOTEDATA_t * conndata;			// client login/pass/subdomain recived data
	DB_USERDATA_t * dbdata;				// database info about particular user
	char * zonepath;					// full path to zone file
	struct subdomain_st * fulldomain; 	// domain and subdomain
	pid_t reload_p;						// pid of bind reload process
	char * timestamp_s;

	while(1) {
		conndata = (REMOTEDATA_t *) malloc(sizeof(REMOTEDATA_t));
		dbdata = (DB_USERDATA_t *) malloc(sizeof(DB_USERDATA_t));

		InitConnData(conndata);
		InitDBData(dbdata);
		/*
		 * Get data sent by ddns-client
		 */
		if((conndata = readCLientData(sockfd, logfd)) == NULL) {
			log_event(logfd, " ERROR reading data from client\n", NULL);
			continue;
		}
		/*
		 * Make connection to database, QUIT process if failed.
		 */
		if((dbh = dbLogin(cfg_file)) == NULL) {
			log_event(logfd, " ERROR: Can't connect to database. QUIT process.\n", NULL);
			return 1;
		}
		/*
		 * Search for user data based on login read in the first step.
		 * Log an error if none was found.
		 */
		if((res = queryUserData(dbh, conndata->login, logfd)) == NULL) {
			log_event(logfd, " ERROR: Unknown user ", conndata->login, "\n", NULL);
			mysql_close(dbh);
			clearConnData(conndata);
			free(conndata);
			continue;
		}
		row = mysql_fetch_row(res);
		/*
		 * Get user details from database
		 */
		dbdata->login = (char *) malloc((strlen(row[1]) + 1) * sizeof(char));
		dbdata->md5 = (char *) malloc((strlen(row[2]) + 1) * sizeof(char));
		dbdata->subdomain = (char *) malloc((strlen(row[3]) + 1) * sizeof(char));
		dbdata->email = (char *) malloc((strlen(row[4]) + 1) * sizeof(char));

		dbdata->id = atoi(row[0]);
		strcpy(dbdata->login, row[1]);
		strcpy(dbdata->md5, row[2]);
		if(row[3][strlen(row[3])-1] == '.')
			row[3][strlen(row[3])-1] = '\0';
		strcpy(dbdata->subdomain, row[3]);
		strcpy(dbdata->email, row[4]);
		dbdata->active = atoi(row[5]);
		/*
		 * Check user password.
		 */
		if(userauth(dbdata, conndata->pass) == 0) {
			log_event(logfd, " ERROR: Incorrect password for ", conndata->login, " try again later\n", NULL);
			mysql_close(dbh);
			clearConnData(conndata);
			free(conndata);
			continue;
		}
		/*
		 * Test if account is active
		 */
		if(dbdata->active == 0) {
			log_event(logfd, " ERROR: Account inactive. Please activate your account!\n", NULL);
			mysql_close(dbh);
			clearConnData(conndata);
			free(conndata);
			continue;
		}
		/*
		 * Check if user can use particular domain
		 */
		if(strcmp(dbdata->subdomain, conndata->subdomain) != 0) {
			log_event(logfd, " ERROR: User ", conndata->login, " is not authorized to use ", conndata->subdomain, "\n", NULL);
			mysql_close(dbh);
			clearConnData(conndata);
			free(conndata);
			continue;
		}
		fulldomain = explodeDomain(conndata->subdomain);	// get domain name.
		/*
		 * obtain full path to zonefile
		 */
		zonepath = (char *) malloc((strlen(cfg_file->server.zonedir) + strlen(fulldomain->dom) + 2) * sizeof(char));
		strcpy(zonepath, cfg_file->server.zonedir);
		strcat(zonepath, fulldomain->dom);
		/*
		 * check if zone file exists
		 */
		if(existZoneFile(zonepath) == 0) {
			log_event(logfd, " ERROR: File: ", zonepath, " not ready, try again later\n", NULL);
			mysql_close(dbh);
			clearConnData(conndata);
			free(conndata);
			continue;
		}
		/*
		 * if client IP address exist -- do nothing
		 */
		if(existEntry(conndata->client_ip_addr, zonepath)) {
			mysql_close(dbh);
			clearConnData(conndata);
			free(conndata);
			continue;
		}
		/*
		 * Update DNS record if exist in zone file
		 */
		if(existEntry(fulldomain->sub, zonepath)){
			if(updateZone(fulldomain->sub, conndata->client_ip_addr, zonepath, logfd)) {
				reload_p = fork();
				if(reload_p == 0)
					namedReload();
				else if(reload_p > 0)
					waitpid(reload_p, NULL, WNOHANG);
				else
					log_event(logfd, " ERROR Reload named failed\n", NULL);
				timestamp_s = timestamp();
				if(dbUpdate(dbh, dbdata, fulldomain, conndata->client_ip_addr, timestamp_s) == 0)
					log_event(logfd, " Error: Database update failed\n", NULL);
				else
					log_event(logfd, "INFO: Domain: ", dbdata->subdomain, " updated\n", NULL);
				free(timestamp_s);
			}
			else
				log_event(logfd, " Error on update zone: ", fulldomain->dom, "\n", NULL);
		}
		/*
		 * we don't need these data anymore.
		 */
		clearConnData(conndata);
		clearDBData(dbdata);
		mysql_close(dbh);
		free(conndata);
		free(dbdata);
	}
	return 0;
}
static void clearConnData(REMOTEDATA_t * conn) {
	free(conn->login);
	free(conn->pass);
	free(conn->subdomain);
}
static void clearDBData(DB_USERDATA_t * db) {
	db->active = 0;
	db->id = 0;
	free(db->email);
	free(db->login);
	free(db->md5);
	free(db->subdomain);
}
