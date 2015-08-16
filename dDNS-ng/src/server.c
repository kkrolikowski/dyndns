#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "dynsrv.h"
#include "common.h"
#include "clientmanager.h"

int main(int argc, char *argv[]) {
	pid_t pid, sess, sync_pid;
	int log_fd, sockfd, status;
	extern char logmsg[LOG_MSG_LEN];
	extern char t_stamp[TIMESTAMP_LEN];
	config_t config;
	char port_str[8];

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
		printf("Starting clientManager. PID: %d\n", pid);
		pidfile(pid, config.pidf);
		exit(0);
	}
	umask(0);
	sess = setsid();
	chdir("/");
	log_fd = open(config.logfile, O_RDWR|O_CREAT|O_APPEND, 0644);
	sockfd = bindToInterface(config.port);
	sprintf(port_str, "%d", config.port);
	if (sockfd < 0) {
		log_event(log_fd, " ERROR: Cannot bind to interface\n", NULL);
		unlink(config.pidf);
		free(config.pidf);
		exit(1);
	}
	else
		log_event(log_fd, " INFO: Listening on port: ", port_str, "\n", NULL);
	sync_pid = fork();
	if(sync_pid < 0) {
		perror("fork");
		exit(-1);
	}
	else if(sync_pid == 0)
		dbsync(&config, log_fd);
	else {
		printf("Starting dbSync. PID: %d\n", sync_pid);
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
		//ddserv(&config, log_fd, sockfd);
		clientManager(&config, log_fd, sockfd);
	}
	close(log_fd);
	free(config.logfile);
	unlink(config.pidf);
	free(config.pidf);
	return 0;
}
