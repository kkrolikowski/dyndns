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

	cfgfile = fopen(filename, "r");
	if(cfgfile == NULL)
		return false;
	while(fgets(buf, sizeof(buf), cfgfile) != NULL) {
		if(buf[0] == '#')
			continue;
		if(strstr(buf, "server ="))
			strcpy(cfg->client.host, getVal(buf));
		if(strstr(buf, "listen_on ="))
			cfg->port = atoi(getVal(buf));
		if(strstr(buf, "domain ="))
			strcpy(cfg->client.domain, getVal(buf));
		if(strstr(buf, "zones ="))
			strcpy(cfg->server.zonedir, getVal(buf));
		if(strstr(buf, "log ="))
			strcpy(cfg->logfile, getVal(buf));
		if(strstr(buf, "interval ="))
			cfg->client.interval = atoi(getVal(buf));
		if(strstr(buf, "login ="))
			strcpy(cfg->client.username, getVal(buf));
		if(strstr(buf, "pass ="))
			strcpy(cfg->client.password, getVal(buf));
		if(strstr(buf, "pid ="))
			strcpy(cfg->pid, getVal(buf));
		if(strstr(buf, "db_host ="))
			strcpy(cfg->server.db_host, getVal(buf));
		if(strstr(buf, "db_name ="))
			strcpy(cfg->server.db_name, getVal(buf));
		if(strstr(buf, "db_user ="))
			strcpy(cfg->server.db_login, getVal(buf));
		if(strstr(buf, "db_secret ="))
			strcpy(cfg->server.db_pass, getVal(buf));
		if(strstr(buf, "smtp_ip ="))
			strcpy(cfg->server.smtp_ip, getVal(buf));
		if(strstr(buf, "smtp_port ="))
			cfg->server.smtp_port = atoi(getVal(buf));
		if(strstr(buf, "mail_from ="))
			strcpy(cfg->server.mail_from, getVal(buf));
	}
	fclose(cfgfile);
	return true;
}
char * getVal(char * str) {
	while(*str++) {
		if(isspace(*str) && (isalnum(*(str+1)) || *(str+1) == '/'))
			break;
	}
	*(str+(strlen(str)-1)) = '\0';
	return ++str;
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
