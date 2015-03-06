#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <stdbool.h>
#include "dynsrv.h"

static void splitDomain(char *userdomain, cfgdata_t * cfg);

int main(int argc, char *argv[]) {
	int sockfd, cli_fd;
	int status;
	int new = 0;
	char * source_addr;
	char * client_domain;
	char zonepath[64];
	cfgdata_t cf;

	source_addr = (char *) malloc(16 * sizeof(char));
	client_domain = (char *) malloc(64 * sizeof(char));
	bzero(source_addr, 16 * sizeof(char));
	bzero(client_domain, 64 * sizeof(char));

	if (argc < 3) {
		fprintf(stderr, "Usage: %s [port number] <zonedir>\n", argv[0]);
		exit(1);
	}
	strcpy(zonepath, argv[2]);
	sockfd = bindToInterface(atoi(argv[1]));
	if (sockfd < 0) {
		fprintf(stderr, "Cannot bind to interface\n");
		exit(1);
	}
	if ((cli_fd = clientConn(sockfd, source_addr)) < 0) {
		fprintf(stderr, "connection failed\n");
		exit(1);
	} else {
		strcpy(cf.ip_addr, source_addr);
		if (readData(cli_fd, client_domain) > 0) {
			splitDomain(client_domain, &cf);
			strcat(zonepath, cf.domain);
		}
		else {
			fprintf(stderr, "Error reading data from client\n");
			exit(1);
		}
	}
	if(if_Exist(cf.subdomain, zonepath) == true) {
		new = 0;
		if(if_Exist(cf.ip_addr, zonepath) == false) {
			updateZone(&cf, zonepath, new);
		}
	}
	else {
		new = 1;
		updateZone(&cf, zonepath, new);
	}
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
