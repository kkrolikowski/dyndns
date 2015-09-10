#ifndef DYNSRV_H_
#define DYNSRV_H_
#include <mysql.h>
#include "common.h"
#define LOG_MSG_LEN 256
#define TIMESTAMP_LEN 32

char logmsg[LOG_MSG_LEN];
char t_stamp[TIMESTAMP_LEN];
MYSQL * dbLogin(config_t * cf);
char * tempFile(int len);
char ** getAdminEmail(MYSQL * dbh);
int sendmail(config_t * cf, char * mailto, char * subject, char * msg);
int dbsync(config_t * cfg, int server_log);
int clientManager(config_t * cfg_file, int logfd, int sockfd);
int namedReload(void);
#endif
