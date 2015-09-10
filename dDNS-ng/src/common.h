#ifndef CONFIG_H_
#define CONFIG_H_
#include <stdbool.h>
#define LOG_MSG_LEN 256				// MAX lenght of a log entry
#define TIMESTAMP_LEN 32			// lenght of timestamp string

/*
 * userconfig_t stores client configuration data
 */
typedef struct userconfig {
	char * host;
	char * domain;
	char * username;
	char * password;
	int interval;
} userconfig_t;
/*
 * serverconfig_t stores server configuration data
 */
typedef struct serverconfig {
	char * zonedir;
	char * db_host;
	char * db_name;
	char * db_login;
	char * db_pass;
	char * smtp_ip;
	int smtp_port;
	char * mail_from;
	char * namedconf;
} SRVCFG_t;
/*
 * config_t common client/server configuration
 */
typedef struct config {
	SRVCFG_t server;
	userconfig_t client;
	char * logfile;
	char * pidf;
	int port;
} config_t;
char logmsg[LOG_MSG_LEN];				// log entry

/*
 * log_event -- put an entry to logfile
 * It takes variable amount of string arguments
 */
void log_event(int logfd, char *first, ...);
/*
 * ReadCFG -- read a configuration file
 * Takes two arguments: pointer to configuration datatype
 * and path to configuration file.
 */
bool ReadCFG(config_t * cfg, char * filename);
/*
 * getVal -- get value from config option
 * Take one arg which is string containing option and it's value
 * Returns option's value
 */
char * getVal(char * str);
/*
 * pidfile -- create a pidfile
 * takes two arguments: process PID and path to pidfile.
 */
int pidfile(pid_t pid, char *name);
char * timestamp(void);
#endif
