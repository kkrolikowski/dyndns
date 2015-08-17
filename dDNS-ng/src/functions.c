#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>
#include <stdbool.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <mysql.h>
#include "dynsrv.h"
#include "auth.h"
#include "common.h"

MYSQL * dbLogin(config_t * cf) {
	MYSQL * db;
	db = mysql_init(NULL);
	if(db == NULL)
		return NULL;
	if(mysql_real_connect(db, cf->server.db_host,
			cf->server.db_login, cf->server.db_pass, cf->server.db_name, 3306, NULL, 0) == NULL)
		return NULL;
	return db;
}
char ** getAdminEmail(MYSQL * dbh) {
	MYSQL_RES * res;
	MYSQL_ROW row;
	int i = 0;
	int resCnt;
	char ** admins;

	char * query = "SELECT email FROM users WHERE role = 'admin'";

	if(mysql_query(dbh, query) != 0) {
		mysql_close(dbh);
		return NULL;
	}
	res = mysql_store_result(dbh);
	if(res == NULL) {
		mysql_close(dbh);
		return NULL;
	}
	if((resCnt = mysql_field_count(dbh)) == 0) {
		free(query);
		mysql_close(dbh);
		return NULL;
	}
	admins = (char **) malloc(resCnt * sizeof(char *));
	while((row = mysql_fetch_row(res)) != 0) {
		admins[i] = malloc((strlen(row[0]) * sizeof(char)) + 1);
		strcpy(admins[i], row[0]);
		i++;
	}
	mysql_free_result(res);

	return admins;
}
int sendmail(config_t * cf, char * mailto, char * subject, char * msg) {
	int smtp_fd;
	int seq = 0;
	struct sockaddr_in smtp_s;
	char buf[1024];
	char rec_buf[256];

	if((smtp_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
		return -1;
	memset(&smtp_s, 0, sizeof(smtp_s));
	smtp_s.sin_family = AF_INET;
	smtp_s.sin_addr.s_addr = inet_addr(cf->server.smtp_ip);
	smtp_s.sin_port = htons(cf->server.smtp_port);

	if(connect(smtp_fd, (struct sockaddr *) &smtp_s, sizeof(smtp_s)) < 0)
		return -1;
	bzero(buf, 1024);
	bzero(rec_buf, 256);
	while(read(smtp_fd, rec_buf, 256) > 0 && (strstr(rec_buf, "220") != NULL || strstr(rec_buf, "250") != NULL || strstr(rec_buf, "354")) && seq < 6) {
		switch(seq) {
			case 0 :
				strcpy(buf, "HELO smtplib-0.1");
				break;
			case 1 :
				 strcpy(buf, "MAIL FROM:<");
				 strcat(buf, cf->server.mail_from);
				 strcat(buf, ">");
				 break;
			case 2 :
				 strcpy(buf, "RCPT TO:<");
				 strcat(buf, mailto);
				 strcat(buf, ">");
				 break;
			case 3 :
				 strcpy(buf, "DATA");
				 break;
			case 4 :
				 strcpy(buf, "X-Author: Krzysztof Krolikowski <kkrolikowski@gmail.com>\n");
				 strcat(buf, "X-Mailer: smtp-lib-0.1\n");
				 strcat(buf, "To: ");
				 strcat(buf, mailto);
				 strcat(buf, "\n");
				 strcat(buf, "Subject: [dDNS] ");
				 strcat(buf, subject);
				 strcat(buf, "\n\n");
				 strcat(buf, msg);
				 strcat(buf, "\n\n--\nYour dDNS service 1.0\n.\n");
				 break;
			case 5 :
				 strcpy(buf, "quit");
				 break;
		}
		write(smtp_fd, buf, sizeof(buf));
		write(smtp_fd, "\r\n", 2);
		seq++;
	}
	close(smtp_fd);

	return 1;
}
int namedReload() {
	int ret;

	ret = execl("/usr/sbin/rndc", "rndc", "reload", NULL);
	if(ret == -1) {
		ret = execl("/usr/local/sbin/rndc", "rndc", "reload", NULL);
		if(ret == -1) {
			perror("execl");
			return 0;
		}
	}
	return 1;
}
