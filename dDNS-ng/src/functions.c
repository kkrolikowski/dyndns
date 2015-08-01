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

int NewEntry(cfgdata_t * cf, char * file) {
    FILE *zf;
    FILE *tmp;
    char tmp_path[21];
    char buf[256];
    char serial[12];
    char newserial[12];

    zf = fopen(file, "r");
    if(zf == NULL) {
            fprintf(stderr, "Error reading file: %s", file);
            return 1;
    }
    tmp = fopen(tmp_path, "w");
    if(tmp == NULL) {
    	fprintf(stderr, "Error creating file: %s", tmp_path);
    	return 1;
    }
    while(fgets(buf, sizeof(buf), zf) != NULL) {
	    if(strstr(buf, "; serial") != NULL) {
//			stripSerialNo(buf, serial);
//			if(!updateSerialNo(serial, newserial))
//				sprintf(buf, "\t%s\t; serial\n", newserial);
	    }
	    fputs(buf, tmp);
    }
	if(strlen(cf->subdomain) < 8)
		sprintf(buf, "%s\t300\tIN\tA\t%s\n", cf->subdomain, cf->ip_addr);
	else
		sprintf(buf, "%s\t300\tIN\tA\t%s\n", cf->subdomain, cf->ip_addr);
	fputs(buf, tmp);

	fclose(zf);
    fclose(tmp);

    //apply(tmp_path, file, cf->domain);
    return 0;
}
bool updateDB(MYSQL * dbh, sqldata_t *info, char *login, char *ip, char * timestamp) {
	char * query;
	char * update_query = "UPDATE subdomains set ip = '";
	query = (char *) malloc((strlen(update_query) + strlen(ip) + strlen(timestamp) + strlen(login) + 128) * sizeof(char));

	strcpy(query, update_query);
	strcat(query, ip);
	strcat(query, "', lastupdate = '");
	strcat(query, timestamp);
	strcat(query,"'");
	strcat(query, " WHERE user_id = (SELECT id FROM users WHERE login = '");
	strcat(query, login);
	strcat(query, "'");
	strcat(query, " AND active = 1) AND type = 'A' AND dynamic = 1");

	 if(mysql_query(dbh, query) != 0) {
	 	return false;
	 }
	 free(query);
	 return true;
}
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
int getUserID(MYSQL * dbh, char * login) {
	MYSQL_RES * res;
	MYSQL_ROW row;
	int userid;
	char * query;
	char * userid_query = "SELECT id FROM users WHERE login = '";
	query = (char *) malloc((strlen(userid_query) + strlen(login) + 2) * sizeof(char));

	strcpy(query, userid_query);
	strcat(query, login);
	strcat(query, "'");

	if(mysql_query(dbh, query) != 0) {
		mysql_close(dbh);
		userid = -1;
	}
	res = mysql_store_result(dbh);
	if(res == NULL) {
		mysql_close(dbh);
		userid = -1;
	}
	if(mysql_field_count(dbh) == 0) {
		free(query);
		mysql_close(dbh);
		return -1;
	}
	if((row = mysql_fetch_row(res)) == 0)
		userid = -1;
	else
		userid = atoi(row[0]);

	free(query);
	return userid;
}
bool userlog(MYSQL * dbh, int userid, char *ip, char * timestamp) {
	char str_userid[2];
	char * query;
	sprintf(str_userid, "%d", userid);
	char * log_query = "INSERT INTO user_log(user_id,ip,date) VALUES(";
	query = (char *) malloc((strlen(log_query) + strlen(ip) + strlen(timestamp) + 12) * sizeof(char));

	strcpy(query, log_query);
	strcat(query, str_userid);
	strcat(query, ", '");
	strcat(query, ip);
	strcat(query,"', '");
	strcat(query, timestamp);
	strcat(query, "')");

	 if(mysql_query(dbh, query) != 0) {
	 	return false;
	 }
	 free(query);
	 return true;
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
