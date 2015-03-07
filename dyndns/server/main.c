#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <stdbool.h>
#include "dynsrv.h"

static void splitDomain(char *userdomain, cfgdata_t * cfg);

int main(int argc, char *argv[]) {
	int sockfd, cli_fd;
	int status;
	char * source_addr;
	char * client_domain;
	char zonepath[64];
	cfgdata_t cf;
	pid_t chld;

	source_addr = (char *) malloc(16 * sizeof(char));
	client_domain = (char *) malloc(64 * sizeof(char));
	bzero(source_addr, 16 * sizeof(char));
	bzero(client_domain, 64 * sizeof(char));

	if (argc < 3) {
		fprintf(stderr, "Usage: %s [port number] <zonedir>\n", argv[0]);
		exit(1);
	}
	chld = fork();
	if(chld < 0) {
		perror("fork");
		exit(-1);
	}
	if(chld > 0) {
		printf("Starting server process\n");
		waitpid(chld, &status, WNOHANG);
	}
	else {
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
			if(if_Exist(cf.ip_addr, zonepath) == false) {
				updateZone(&cf, zonepath);
			}
		}
		else {
			NewEntry(&cf, zonepath);
		}
		free(source_addr);
		free(client_domain);
		exit(0);
	}
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
