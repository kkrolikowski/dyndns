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
MYSQL_RES * queryUserData(MYSQL * dbh, char * login, int logger);
REMOTEDATA_t * readCLientData(int sockfd, int logger);
struct conn_st * clientConn(int sock);
