#ifndef CLIENTMANAGER_H_
#define CLIENTMANAGER_H_
#include <mysql.h>

typedef struct remotedata {
	char * login;
	char * pass;
	char * subdomain;
	char * client_ip_addr;
} REMOTEDATA_t;
typedef struct db_userdata {
	int id;
	char * login;
	char * md5;
	char * subdomain;
	char * email;
	int active;
} DB_USERDATA_t;
struct conn_st {
	int fd;
	char * client_ip;
};
struct subdomain_st {
	char * sub;
	char * dom;
};
MYSQL_RES * queryUserData(MYSQL * dbh, char * login, int logger);
REMOTEDATA_t * readCLientData(int sockfd, int logger);
struct conn_st * clientConn(int sock);
int bindToInterface(int portno);
char * stripDomain(char * subdomain);
int existZoneFile(char * filepath);
struct subdomain_st * explodeDomain(char * fulldomain);
int existEntry(char * what, char * where);
char * stripSerialNo(char * input);
char * newSerialNo(char * serial);
int updateZone(char * subdomain, char * ipaddr, char * file, int logger);
void InitConnData(REMOTEDATA_t * conn);
void InitDBData(DB_USERDATA_t * db);
char * tempFile(int len);
#endif
