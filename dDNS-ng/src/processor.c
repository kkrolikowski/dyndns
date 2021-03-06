#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
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
	struct subdomain_st * clientDomain; 	// domain and subdomain
	pid_t reload_p;						// pid of bind reload process
	char * timestamp_s;
	size_t zonepathLen = 0;
	size_t ipchange_msg_len = 0;		// custom message lenght
	char * clientDomain_str;			// string form of userdomain

	/*
	 *  building custom message to send to user
	 */
	char * ipchange_msg_1 = "Dear ";
	char * ipchange_msg_2 = "\nDomain: ";
	char * ipchange_msg_3 = " has IP: ";
	char * ipchange_msg;
	ipchange_msg_len = strlen(ipchange_msg_1) + strlen(ipchange_msg_2) + strlen(ipchange_msg_3);

	while(1) {
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
		dbdata = (DB_USERDATA_t *) malloc(sizeof(DB_USERDATA_t));
		InitDBData(dbdata);
		/*
		 * Search for user data based on login read in the first step.
		 * Log an error if none was found.
		 */
		if((res = queryUserData(dbh, conndata->login, logfd)) == NULL) {
			if(conndata->login != NULL)
                log_event(logfd, " ERROR: Unknown user ", conndata->login, "\n", NULL);
            else
                log_event(logfd, " ERROR: Unknown user from ", inet_ntoa(conndata->client_ip_addr), "\n", NULL);
			mysql_free_result(res);
			mysql_close(dbh);
			clearConnData(conndata);
			free(dbdata);
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
		dbdata->serial = (char *) malloc((strlen(row[7]) + 1) * sizeof(char));
		dbdata->domstatus = (char *) malloc((strlen(row[8]) + 1) * sizeof(char));

		dbdata->id = atoi(row[0]);
		strcpy(dbdata->login, row[1]);
		strcpy(dbdata->md5, row[2]);
		if(row[3][strlen(row[3])-1] == '.')
			row[3][strlen(row[3])-1] = '\0';
		strcpy(dbdata->subdomain, row[3]);
		strcpy(dbdata->email, row[4]);
		dbdata->active = atoi(row[5]);
		strcpy(dbdata->domstatus, row[8]);

		mysql_free_result(res);
		/*
		 * Check user password.
		 */
		if(userauth(dbdata->md5, conndata->pass) == 0) {
			log_event(logfd, " ERROR: Incorrect password for ", conndata->login, " try again later\n", NULL);
			mysql_close(dbh);
			clearConnData(conndata);
			clearDBData(dbdata);
			continue;
		}
		/*
		 * Test if account is active
		 */
		if(dbdata->active == 0) {
			log_event(logfd, " ERROR: Account inactive. Please activate your account!\n", NULL);
			mysql_close(dbh);
			clearConnData(conndata);
			clearDBData(dbdata);
			continue;
		}
		clientDomain = explodeDomain(conndata->subdomain);	// get domain name.
		clientDomain_str = (char *) malloc((clientDomain->len +2) * sizeof(char));
		strcpy(clientDomain_str, clientDomain->sub);
		strcat(clientDomain_str, ".");
		strcat(clientDomain_str, clientDomain->dom);

		/*
		 * Check if user can use particular domain
		 */
		if(strcmp(dbdata->subdomain, clientDomain_str) != 0) {
			log_event(logfd, " ERROR: User ", conndata->login, " is not authorized to use ", clientDomain_str, "\n", NULL);
			mysql_close(dbh);
			clearConnData(conndata);
			clearDBData(dbdata);
			free(clientDomain_str);
			continue;
		}
		free(clientDomain_str);
		/*
		 * obtain full path to zonefile
		 */
		zonepathLen = strlen(cfg_file->server.zonedir) + strlen(clientDomain->dom) + 1;
		zonepath = (char *) malloc(zonepathLen * sizeof(char));
		bzero(zonepath, zonepathLen);
		memset(zonepath, 0, zonepathLen);
		strcpy(zonepath, cfg_file->server.zonedir);
		strcat(zonepath, clientDomain->dom);
		/*
		 * check if zone file exists
		 */
		if(existZoneFile(zonepath) == 0) {
			log_event(logfd, " ERROR: File: ", zonepath, " not ready, try again later\n", NULL);
			mysql_close(dbh);
			clearConnData(conndata);
			clearDBData(dbdata);
			free(zonepath);
			free(clientDomain->dom);
			free(clientDomain->sub);
			free(clientDomain);
			continue;
		}
		/*
		 * if client IP address exist -- do nothing
		 */
		if(existEntry(inet_ntoa(conndata->client_ip_addr), zonepath)) {
			if(clientDomain->sub[0] == '@')
				log_event(logfd, " INFO: ", clientDomain->dom, " is up to date\n", NULL);
			else
				log_event(logfd, " INFO: ", conndata->subdomain, " is up to date\n", NULL);
			mysql_close(dbh);
			clearConnData(conndata);
			clearDBData(dbdata);
			free(zonepath);
			free(clientDomain->dom);
			free(clientDomain->sub);
			free(clientDomain);
			continue;
		}
		/*
		 * Update DNS record if exist in zone file
		 */
		if(existEntry(clientDomain->sub, zonepath)) {
			if(updateZone(clientDomain->sub, inet_ntoa(conndata->client_ip_addr), dbdata->serial, zonepath, logfd)) {
				reload_p = fork();
				if(reload_p == 0)
					namedReload();
				else if(reload_p > 0)
					waitpid(reload_p, NULL, WNOHANG);
				else
					log_event(logfd, " ERROR Reload named failed\n", NULL);
				timestamp_s = timestamp();
				if(dbUpdate(dbh, dbdata, clientDomain, inet_ntoa(conndata->client_ip_addr), timestamp_s) == 0)
					log_event(logfd, " Error: Database update failed\n", NULL);
				else {
					ipchange_msg = (char *) malloc((ipchange_msg_len + strlen(dbdata->login) + strlen(conndata->subdomain) + strlen(inet_ntoa(conndata->client_ip_addr)) + 2) * sizeof(char));
					strcpy(ipchange_msg, ipchange_msg_1);
					strcat(ipchange_msg, dbdata->login);
					strcat(ipchange_msg, ipchange_msg_2);
					strcat(ipchange_msg, conndata->subdomain);
					strcat(ipchange_msg, ipchange_msg_3);
					strcat(ipchange_msg, inet_ntoa(conndata->client_ip_addr));
					sendmail(cfg_file, dbdata->email, "IP Address Update", ipchange_msg);
					free(ipchange_msg);
					if(clientDomain->sub[0] == '@')
						log_event(logfd, " INFO: Domain: ", clientDomain->dom, " updated\n", NULL);
					else
						log_event(logfd, " INFO: Domain: ", dbdata->subdomain, " updated\n", NULL);
				}
				free(timestamp_s);
			}
			else
				log_event(logfd, " Error on update zone: ", clientDomain->dom, "\n", NULL);
		}
		/*
		 * adding subdomains by fresh users.
		*/
		else {
            if(strcmp(dbdata->domstatus, "public") == 0) {
                if(appendDomain(zonepath, clientDomain->sub, inet_ntoa(conndata->client_ip_addr)) == 0) {
                    log_event(logfd, " ERROR: append zone: ", zonepath, " failed\n", NULL);
                    mysql_close(dbh);
                    clearConnData(conndata);
                    clearDBData(dbdata);
                    free(zonepath);
                    free(clientDomain->dom);
                    free(clientDomain->sub);
                    free(clientDomain);
                }
                else {
                    timestamp_s = timestamp();
                    if(dbUpdate(dbh, dbdata, clientDomain, inet_ntoa(conndata->client_ip_addr), timestamp_s) == 0)
                        log_event(logfd, " Error: Database update failed\n", NULL);
                    free(timestamp_s);
                    log_event(logfd, " INFO: New subdomain: ", dbdata->subdomain, "\n", NULL);
                    reload_p = fork();
                    if(reload_p == 0)
                        namedReload();
                    else if(reload_p > 0)
                        waitpid(reload_p, NULL, WNOHANG);
                    else
                        log_event(logfd, " ERROR Reload named failed\n", NULL);
                }
            }
		}
		/*
		 * we don't need these data anymore.
		 */
		mysql_close(dbh);
		clearConnData(conndata);
		clearDBData(dbdata);
		free(zonepath);
		free(clientDomain->dom);
		free(clientDomain->sub);
		free(clientDomain);
	}
	return 0;
}
static void clearConnData(REMOTEDATA_t * conn) {
	free(conn->login);
	free(conn->pass);
	free(conn->subdomain);
	free(conn);
}
static void clearDBData(DB_USERDATA_t * db) {
	db->active = 0;
	db->id = 0;
	free(db->email);
	free(db->login);
	free(db->md5);
	free(db->subdomain);
	free(db->serial);
	free(db->domstatus);
	free(db);
}
