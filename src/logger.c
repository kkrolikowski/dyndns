#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <fcntl.h>
#include "logger.h"

void log_event(int logfd, char *first, ...) {
	va_list msgs;
	char *next = first;
	extern char logmsg[LOG_MSG_LEN];
	extern char t_stamp[TIMESTAMP_LEN];

	va_start(msgs, first);
	strcpy(logmsg, timestamp_new(t_stamp));
	while(next != NULL) {
		next = va_arg(msgs, char *);
		strcat(logmsg, next);
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
