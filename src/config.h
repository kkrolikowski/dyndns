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
	int interval;
} userconfig_t;
typedef struct serverconfig {
	char zonedir[FPATH_MAX];
	char logfile[FPATH_MAX];
} serverconfig_t;
typedef struct config {
	serverconfig_t server;
	userconfig_t client;
	int port;
} config_t;

bool ReadCFG(config_t * cfg, char * filename);
char * getVal(char * str);
#endif
