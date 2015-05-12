#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/uio.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <mysql.h>
#include "dynsrv.h"
#include "auth.h"
#include "common.h"

static void splitDomain(char *userdomain, cfgdata_t * cfg);

int ddserv(config_t * cfg_file, int logfd, int sockfd) {
	int cli_fd, n;
	int authstatus;
	int dynuser_id;
	char * source_addr;
	char login[12];
	char pass[24];
	char client_domain[64];
	char zonepath[64];
	cfgdata_t cf;
	sqldata_t db_userdata;
	extern char t_stamp[TIMESTAMP_LEN];
	struct iovec client_data[3];
	MYSQL * dbh;

	source_addr = (char *) malloc(16 * sizeof(char));
	bzero(source_addr, 16 * sizeof(char));
	client_data[0].iov_base = login;
	client_data[0].iov_len = sizeof(login);
	client_data[1].iov_base = pass;
	client_data[1].iov_len = sizeof(pass);
	client_data[2].iov_base = client_domain;
	client_data[2].iov_len = sizeof(client_domain);

	strcpy(zonepath, cfg_file->server.zonedir);
	if ((cli_fd = clientConn(sockfd, source_addr)) < 0) {
		log_event(logfd, " ERROR: Connection failed from: ", source_addr, "\n", NULL);
		exit(-1);
	}
	else {
		log_event(logfd, " INFO: Client: ", source_addr, " connected\n", NULL);
		strcpy(cf.ip_addr, source_addr);
	}
	n = readv(cli_fd, client_data, 3);
	if(n < 0) {
		log_event(logfd, " ERROR: Read data failed from: ", source_addr, "\n", NULL);
		exit(-1);
	}
	if((dbh = dbLogin(cfg_file)) == NULL) {
			log_event(logfd, " ERROR: DB connection failed.\n", NULL);
			exit(-1);
	}
	if(getUserData(dbh, &db_userdata, login) == false) {
		log_event(logfd, " ERROR: Fetch SQL Data\n", NULL);
		exit(-1);
	}
	authstatus = userauth(&db_userdata, login, pass);
	dynuser_id = getUserID(dbh, login);
	if(authstatus == 0) {
		splitDomain(client_domain, &cf);
		strcat(zonepath, cf.domain);
		if(if_Exist(cf.subdomain, zonepath) == true) {
			if(if_Exist(cf.ip_addr, zonepath) == false) {
				if(isAuthorized(&db_userdata, login, client_domain) > 0) {
					updateZone(&cf, zonepath);
					updateDB(dbh, &db_userdata, login, source_addr, timestamp_new(t_stamp));
					userlog(dbh, dynuser_id, source_addr, timestamp_new(t_stamp));
					log_event(logfd, " INFO: ", cf.subdomain, " IP Address updated\n", NULL);
				}
				else
					log_event(logfd, " ERROR: ", cf.subdomain, " update failed\n", NULL);
			}
		}
		else {
			if(isAuthorized(&db_userdata, login, client_domain) > 0) {
				NewEntry(&cf, zonepath);
				updateDB(dbh, &db_userdata, login, source_addr, timestamp_new(t_stamp));
				userlog(dbh, dynuser_id, source_addr, timestamp_new(t_stamp));
				log_event(logfd, " INFO: New host added: ", cf.subdomain, ".", cf.domain, "\n", NULL);
			}
			else
				log_event(logfd, " ERROR: You are not allowed to create: ", cf.subdomain, ".", cf.domain, "\n", NULL);
		}
	}
	else if(authstatus == 1)
		log_event(logfd, " ERROR: Unknown user: ", login, "\n", NULL);
	else if(authstatus == 2)
		log_event(logfd, " ERROR: Incorrect password for user: ", login, "\n", NULL);
	close(cli_fd);
	free(source_addr);
	mysql_close(dbh);
	exit(1);
}
static void splitDomain(char *userdomain, cfgdata_t * cfg) {
    char * dom;
    char * subdom;

    dom = cfg->domain;
    subdom = cfg->subdomain;

    while(*userdomain) {
            *subdom++ = *userdomain++;
            if(*userdomain == '.') {
                    *subdom = '\0';
                    break;
            }
    }
    while(*++userdomain)
            *dom++ = *userdomain;
    *dom = '\0';
}
