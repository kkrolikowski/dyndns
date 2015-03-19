#ifndef CONFIG_H_
#define CONFIG_H_
#include <stdbool.h>
#define LOG_MSG_LEN 256
#define TIMESTAMP_LEN 32
#define FPATH_MAX 256

typedef struct clientdata {
	char domain[64];
	char subdomain[16];
	char ip_addr[16];
} clientdata_t;
typedef struct userconfig {
	char host[50];
	char domain[64];
	char username[12];
	char password[24];
	int interval;
} userconfig_t;
typedef struct serverconfig {
	char zonedir[FPATH_MAX];
} serverconfig_t;
typedef struct config {
	serverconfig_t server;
	userconfig_t client;
	char logfile[FPATH_MAX];
	char pid[FPATH_MAX];
	int port;
} config_t;
char logmsg[LOG_MSG_LEN];
char t_stamp[TIMESTAMP_LEN];

char * timestamp_new(char * t_stamp);
void log_event(int logfd, char *first, ...);
bool ReadCFG(config_t * cfg, char * filename);
char * getVal(char * str);
int pidfile(pid_t pid, char *name);
#endif
