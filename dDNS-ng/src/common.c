#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include "common.h"

bool ReadCFG(config_t * cfg, char * filename) {
	FILE * cfgfile;
	char buf[256];
	char * val;

	cfgfile = fopen(filename, "r");
	if(cfgfile == NULL)
		return false;
	while(fgets(buf, sizeof(buf), cfgfile) != NULL) {
		if(buf[0] == '#')
			continue;
		if(strstr(buf, "server =")) {
			val = getVal(buf);
			cfg->client.host = (char *) malloc((strlen(val) + 1) * sizeof(char));
			strcpy(cfg->client.host, val);
			free(val);
		}
		if(strstr(buf, "listen_on =") != NULL || strstr(buf, "srv_port =") != NULL) {
			val = getVal(buf);
			cfg->port = atoi(val);
			free(val);
		}
		if(strstr(buf, "domain =")) {
			val = getVal(buf);
			cfg->client.domain = (char *) malloc((strlen(val) + 1) * sizeof(char));
			strcpy(cfg->client.domain, val);
			free(val);
		}
		if(strstr(buf, "zones =")) {
			val = getVal(buf);
			cfg->server.zonedir = (char *) malloc((strlen(val) + 1) * sizeof(char));
			strcpy(cfg->server.zonedir, val);
			free(val);
		}
		if(strstr(buf, "named_conf =")) {
			val = getVal(buf);
			cfg->server.namedconf = (char *) malloc((strlen(val) + 1) * sizeof(char));
			strcpy(cfg->server.namedconf, val);
			free(val);
		}
		if(strstr(buf, "log =")) {
			val = getVal(buf);
			cfg->logfile = (char *) malloc((strlen(val) + 1) * sizeof(char));
			strcpy(cfg->logfile, val);
			free(val);
		}
		if(strstr(buf, "interval =")) {
			val = getVal(buf);
			cfg->client.interval = atoi(val);
			free(val);
		}
		if(strstr(buf, "login =")) {
			val = getVal(buf);
			cfg->client.username = (char *) malloc((strlen(val) + 1) * sizeof(char));
			strcpy(cfg->client.username, val);
			free(val);
		}
		if(strstr(buf, "pass =")) {
			val = getVal(buf);
			cfg->client.password = (char *) malloc((strlen(val) + 1) * sizeof(char));
			strcpy(cfg->client.password, val);
			free(val);
		}
		if(strstr(buf, "pid =")) {
			val = getVal(buf);
			cfg->pidf = (char *) malloc((strlen(val) + 1) * sizeof(char));
			strcpy(cfg->pidf, val);
			free(val);
		}
		if(strstr(buf, "db_host =")) {
			val = getVal(buf);
			cfg->server.db_host = (char *) malloc((strlen(val) + 1) * sizeof(char));
			strcpy(cfg->server.db_host, val);
			free(val);
		}
		if(strstr(buf, "db_name =")) {
			val = getVal(buf);
			cfg->server.db_name = (char *) malloc((strlen(val) + 1) * sizeof(char));
			strcpy(cfg->server.db_name, val);
			free(val);
		}
		if(strstr(buf, "db_user =")) {
			val = getVal(buf);
			cfg->server.db_login = (char *) malloc((strlen(val) + 1) * sizeof(char));
			strcpy(cfg->server.db_login, val);
			free(val);
		}
		if(strstr(buf, "db_secret =")) {
			val = getVal(buf);
			cfg->server.db_pass = (char *) malloc((strlen(val) + 1) * sizeof(char));
			strcpy(cfg->server.db_pass, val);
			free(val);
		}
		if(strstr(buf, "smtp_ip =")) {
			val = getVal(buf);
			cfg->server.smtp_ip = (char *) malloc((strlen(val) + 1) * sizeof(char));
			strcpy(cfg->server.smtp_ip, val);
			free(val);
		}
		if(strstr(buf, "smtp_port =")) {
			val = getVal(buf);
			cfg->server.smtp_port = atoi(val);
			free(val);
		}
		if(strstr(buf, "mail_from =")) {
			val = getVal(buf);
			cfg->server.mail_from = (char *) malloc((strlen(val) + 1) * sizeof(char));
			strcpy(cfg->server.mail_from, val);
			free(val);
		}
	}
	fclose(cfgfile);
	return true;
}
char * getVal(char * str) {
	char * val;
	char * start;

	int i;
	int max = 0;
	while(*str++) {
		if(isspace(*str) && (isalnum(*(str+1)) || *(str+1) == '/'))
			break;
	}
	start = ++str;
	while(*str++ != '\n')
		max++;
	val = (char *) malloc((max+1) * sizeof(char));
	for(i = 0; i < max; i++)
		*(val+i) = *(start+i);
	val[i] = '\0';

	return val;
}
void log_event(int logfd, char *first, ...) {
	va_list msgs;
	char *next = first;
	extern char logmsg[LOG_MSG_LEN];
	extern char t_stamp[TIMESTAMP_LEN];

	va_start(msgs, first);
	strcpy(logmsg, timestamp_new(t_stamp));
	while(next != NULL) {
		strcat(logmsg, next);
		next = va_arg(msgs, char *);
	}
	va_end(msgs);
	write(logfd, logmsg, strlen(logmsg));
}
char * timestamp_new(char * t_stamp) {
    time_t sec;
    struct tm st_time;

    time(&sec);
    localtime_r(&sec, &st_time);
    strftime(t_stamp, TIMESTAMP_LEN * sizeof(t_stamp), "[%d/%B/%Y %T]", &st_time);

    return t_stamp;
}
int pidfile(pid_t pid, char *path) {
	int pfd;
	char buf[16];

	sprintf(buf, "%d", (int) pid);
	pfd = open(path, O_CREAT|O_RDWR, 0644);
	write(pfd, buf, strlen(buf)+1);
	
	close(pfd);
	return pfd;
}
