#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include "dynsrv.h"

static void splitDomain(char *userdomain, cfgdata_t * cfg);

int ddserv(char * portno, char * zonedir, int logfd, int sockfd) {
	int cli_fd;
	char * source_addr;
	char * client_domain;
	char zonepath[64];
	extern char logmsg[LOG_MSG_LEN];
	extern char t_stamp[TIMESTAMP_LEN];
	cfgdata_t cf;

	source_addr = (char *) malloc(16 * sizeof(char));
	client_domain = (char *) malloc(64 * sizeof(char));
	bzero(source_addr, 16 * sizeof(char));
	bzero(client_domain, 64 * sizeof(char));

	strcpy(zonepath, zonedir);
	if ((cli_fd = clientConn(sockfd, source_addr)) < 0) {
		sprintf(logmsg, "%s ERROR: Connection failed from: %s\n", timestamp(t_stamp), source_addr);
		write(logfd, logmsg, strlen(logmsg));
		exit(1);
	}
	else {
		sprintf(logmsg, "%s INFO: Client: %s connected\n", timestamp(t_stamp), source_addr);
		write(logfd, logmsg, strlen(logmsg));
		strcpy(cf.ip_addr, source_addr);
		if (readData(cli_fd, client_domain) > 0) {
			splitDomain(client_domain, &cf);
			strcat(zonepath, cf.domain);
		}
		else {
			sprintf(logmsg, "%s ERROR: Read data failed from: %s\n", timestamp(t_stamp), source_addr);
			write(logfd, logmsg, strlen(logmsg));
			exit(1);
		}
	}
	if(if_Exist(cf.subdomain, zonepath) == true) {
		if(if_Exist(cf.ip_addr, zonepath) == false) {
			if(updateZone(&cf, zonepath)) {
				sprintf(logmsg, "%s INFO: %s IP Address updated\n", timestamp(t_stamp), cf.subdomain);
				write(logfd, logmsg, strlen(logmsg));
			}
			else {
				sprintf(logmsg, "%s ERROR: %s update failed\n", timestamp(t_stamp), cf.subdomain);
				write(logfd, logmsg, strlen(logmsg));
			}
		}
	}
	else
		NewEntry(&cf, zonepath);

	free(source_addr);
	free(client_domain);
	return 0;
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
