#ifndef CONFIG_H_
#define CONFIG_H_
#include <stdbool.h>
#define LOG_MSG_LEN 256				// MAX lenght of a log entry
#define TIMESTAMP_LEN 32			// lenght of timestamp string
#define FPATH_MAX 256				// lenght of file path

/*
 * userconfig_t stores client configuration data
 */
typedef struct userconfig {
	char host[50];
	char domain[64];
	char username[12];
	char password[24];
	int interval;
} userconfig_t;
/*
 * serverconfig_t stores server configuration data
 */
typedef struct serverconfig {
	char zonedir[FPATH_MAX];
} serverconfig_t;
/*
 * config_t common client/server configuration
 */
typedef struct config {
	serverconfig_t server;
	userconfig_t client;
	char logfile[FPATH_MAX];
	char pid[FPATH_MAX];
	int port;
} config_t;
char logmsg[LOG_MSG_LEN];				// log entry
char t_stamp[TIMESTAMP_LEN];			// timestamp

/*
 * timestamp_new -- returns current date and time
 * it take one argument - string which stores formatted
 * timestamp
 */
char * timestamp_new(char * t_stamp);
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
#endif
