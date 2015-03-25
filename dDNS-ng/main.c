#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include "main.h"

int main(void) {
	MYSQL * sql;
	MYSQL * conn;
	MYSQL_RES * res;
	MYSQL_ROW row;

	sql = mysql_init(NULL);
	if(sql == NULL) {
		fprintf(stderr, "Initialization failed\n");
		exit(1);
	}
	conn = mysql_real_connect(sql, SQL_HOST, SQL_LOGIN, SQL_PASS, SQL_DB, 3306, NULL, 0);
	if(conn == NULL) {
		fprintf(stderr, "Error %s\n", mysql_error(sql));
		exit(1);
	}
	if(mysql_query(sql, "SELECT * FROM users") != 0) {
		fprintf(stderr, "ERROR: %s\n", mysql_error(sql));
		exit(1);
	}
	res = mysql_store_result(sql);
	if(res == NULL) {
		fprintf(stderr, "ERROR: %s\n", mysql_error(sql));
		exit(1);
	}
	while((row = mysql_fetch_row(res)) != 0)
		printf("%s %s\n", row[1], row[2]);
	mysql_free_result(res);
	return 0;
}

