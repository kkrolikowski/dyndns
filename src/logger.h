#ifndef LOGGER_H_
#define LOGGER_H_
#define TIMESTAMP_LEN 32
#define LOG_MSG_LEN 256

char logmsg[LOG_MSG_LEN];
char t_stamp[TIMESTAMP_LEN];

char * timestamp_new(char * t_stamp);
void log_event(int logfd, char *first, ...);
#endif
