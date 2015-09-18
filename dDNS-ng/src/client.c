#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>
#include "common.h"

int main(int argc, char *argv[]) {
	config_t config;
	pid_t pid, sess;
	int sockfd, portno, n, logfd, pidfd;
	char * pidf;
	struct sockaddr_in srv_addr;
	struct hostent *server;
	int delay = 1;
	char buf[256];
	int seq = 0;

	if(argc < 3) {
		fprintf(stderr, "%s -c <configfile>\n\t -h print this help\n", argv[0]);
		exit(1);
	}
	if(strcmp(argv[1], "-c") != 0) {
		fprintf(stderr, "%s -c <configfile>\n\t -h print this help\n", argv[0]);
		exit(1);
	}
	if(ReadCFG(&config, argv[2]) == false) {
		fprintf(stderr, "Error reading file: %s\n", argv[2]);
		exit(1);
	}
	pid = fork();
	if(pid < 0) {
		perror("fork");
		exit(-1);
	}
	if(pid > 0) {
		printf("Starting DynDNS client, PID: %d\n", pid);
		pidfd = pidfile(pid, config.pidf);
		exit(0);
	}
	umask(0);
	sess = setsid();
	chdir("/");
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	portno = config.port;

	logfd = open(config.logfile, O_RDWR|O_CREAT|O_APPEND, 0644);
	while(1) {
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if(sockfd < 0)
			log_event(logfd, " Error opening socket\n", NULL);
		server = gethostbyname(config.client.host);
		if(server == NULL) {
			log_event(logfd, " Unknown host\n", NULL);
			close(sockfd);
			unlink(config.pidf);
			free(config.pidf);
			exit(1);
		}
		bzero((char *) &srv_addr, sizeof(srv_addr));
		srv_addr.sin_family = AF_INET;
		bcopy((char *) server->h_addr, (char *) &srv_addr.sin_addr.s_addr, server->h_length);
		srv_addr.sin_port = htons(portno);
		if(connect(sockfd, (struct sockaddr *) &srv_addr, sizeof(srv_addr)) < 0) {
			log_event(logfd, " Error connecting to server, retrying...\n", NULL);
			close(sockfd);
			delay++;
			sleep(config.client.interval * delay);
			continue;
		}
		else {
			if(delay > 1)
				log_event(logfd, " Connected to server, sending data\n", NULL);
			delay = 1;
		}
		bzero(buf, 256);
		while(read(sockfd, buf, 256) > 0 && (strstr(buf, "ACK") != NULL || strstr(buf, "dDNS-ng") != NULL) && seq < 4) {
			switch(seq) {
				case 0 :
					strcpy(buf, "LOGIN ");
					strcat(buf, config.client.username);
					break;
				case 1 :
					strcpy(buf, "PASS ");
					strcat(buf, config.client.password);
					break;
				case 2 :
					strcpy(buf, "SUBDOMAIN ");
					strcat(buf, config.client.domain);
					break;
				case 3 :
					strcpy(buf, "QUIT");
					break;
			}
			write(sockfd, buf, 256);
			seq++;
			bzero(buf, 256);
		}
		seq = 0;
		close(sockfd);
		sleep(config.client.interval);
	}
	close(logfd);
	unlink(config.pidf);
	free(config.pidf);
	return 0;
}
