#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include "common.h"
#include "dynsrv.h"

int dbsync(config_t * cfg, int server_log) {
	MYSQL * dbh;
	char * query = "SELECT * FROM subdomains";

	log_event(server_log, " INFO: synchronization process is ready\n", NULL);
	while(1) {
		dbh = dbLogin(cfg);
		if(dbh == NULL) {
			log_event(server_log, " ERROR Error connecting to database\n", NULL);
			exit(-1);
		}
		mysql_query(dbh, query);
		mysql_close(dbh);
		sleep(15);
	}
	return 1;
}
