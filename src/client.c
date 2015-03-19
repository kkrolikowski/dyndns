#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/uio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>
#include "common.h"

int main(int argc, char *argv[]) {
	config_t config;
	pid_t pid, sess;
	int sockfd, portno, n, logfd;
	struct sockaddr_in srv_addr;
	struct hostent *server;
	struct iovec config_data[3];

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
		exit(0);
	}
	umask(0);
	sess = setsid();
	chdir("/");
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	portno = config.port;

	config_data[0].iov_base = config.client.username;
	config_data[0].iov_len = sizeof(config.client.username);
	config_data[1].iov_base = config.client.password;
	config_data[1].iov_len = sizeof(config.client.password);
	config_data[2].iov_base = config.client.domain;
	config_data[2].iov_len = sizeof(config.client.domain);

	logfd = open(config.logfile, O_RDWR|O_CREAT|O_APPEND, 0644);
	while(1) {
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if(sockfd < 0)
			log_event(logfd, " Error opening socket\n", NULL);
		server = gethostbyname(config.client.host);
		if(server == NULL) {
			log_event(logfd, " Unknown host\n", NULL);
			close(sockfd);
			exit(1);
		}
		bzero((char *) &srv_addr, sizeof(srv_addr));
		srv_addr.sin_family = AF_INET;
		bcopy((char *) server->h_addr, (char *) &srv_addr.sin_addr.s_addr, server->h_length);
		srv_addr.sin_port = htons(portno);
		if(connect(sockfd, (struct sockaddr *) &srv_addr, sizeof(srv_addr)) < 0) {
			log_event(logfd, " Error connecting to server, retrying...\n", NULL);
			continue;
		}
		n = writev(sockfd, config_data, 3);
		if(n < 0) {
			log_event(logfd, " Error sending data.\n", NULL);
			exit(1);
		}
		close(sockfd);
		sleep(config.client.interval);
	}
	close(logfd);
	return 0;
}
