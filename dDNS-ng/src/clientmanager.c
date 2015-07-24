#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <mysql.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "clientmanager.h"

static char * getdata(char *);

REMOTEDATA_t * readCLientData(int sockfd, int logger) {
	int clifd;
	REMOTEDATA_t * data;
	struct conn_st * conn_data;

	char * welcome = "100 dDNS-ng server ready. Go ahead\n";
	char * unknown = "Unknown command\n";
	char * ack = "ACK\n";
	char * val;

	char readbuf[256];

	data = (REMOTEDATA_t *) malloc(sizeof(REMOTEDATA_t));
	bzero(readbuf, 256);

	if((conn_data = clientConn(sockfd)) == NULL) {
		log_event(logger, " ERROR: can't establish connection");
		return NULL;
	}
	data->client_ip_addr = conn_data->client_ip;

	write(conn_data->fd, welcome, strlen(welcome) +1);
	while(read(conn_data->fd, readbuf, 256) > 0) {
		if(strstr(readbuf, "QUIT") != NULL)
			break;
		else if(strstr(readbuf, "LOGIN") != NULL) {
			val = getdata(readbuf);
			data->login = (char *) malloc((strlen(val) + 1) * sizeof(char));
			strcpy(data->login, val);
			write(conn_data->fd, ack, strlen(ack)+1);
			free(val);
		}
		else if(strstr(readbuf, "PASS") != NULL) {
			val = getdata(readbuf);
			data->pass = (char *) malloc((strlen(val) + 1) * sizeof(char));
			strcpy(data->pass, val);
			write(conn_data->fd, ack, strlen(ack)+1);
			free(val);
		}
		else if(strstr(readbuf, "SUBDOMAIN") != NULL) {
			val = getdata(readbuf);
			data->subdomain = (char *) malloc((strlen(val) + 1) * sizeof(char));
			strcpy(data->subdomain, val);
			write(conn_data->fd, ack, strlen(ack)+1);
			free(val);
		}
		else {
			write(conn_data->fd, unknown, strlen(unknown)+1);
			continue;
		}
		bzero(readbuf, 256);
	}
	close(conn_data->fd);
	return data;
}
struct conn_st * clientConn(int sock) {
	int clilen, clifd;
	struct sockaddr_in cli_addr;
	struct conn_st * conn;

	clilen = sizeof(cli_addr);
	clifd = accept(sock, (struct sockaddr *) &cli_addr, &clilen);

	if(clifd < 0)
		return NULL;
	else {
		conn->client_ip = (char *) malloc((strlen((char *) inet_ntoa(cli_addr.sin_addr)) + 1) * sizeof(char));
		strcpy(conn->client_ip, (char *) inet_ntoa(cli_addr.sin_addr));
		conn->fd = clifd;
		return conn;
	}
}
MYSQL_RES * queryUserData(MYSQL * dbh, char * login, int logger) {
	MYSQL_RES * res;
	char * query_prefix = "SELECT u.id, u.login, u.pass, CONCAT(s.subdomain, \".\",  d.domain) as subdomain, \
			u.active, s.dynamic FROM subdomains s, domains d , users u \
			WHERE s.domain_id = d.id and u.id = s.user_id and u.login = '";
	char * query_suffix = "' AND dynamic = 1";
	char * query;

/*
 * build SQL query to get user account information
 */
	query = (char *) malloc((strlen(query_prefix) + strlen(login) + strlen(query_suffix) + 3) * sizeof(char));
	strcpy(query, query_prefix);
	strcat(query, login);
	strcat(query, query_suffix);
/*
 * send query to MySQL server
 */
	if(mysql_query(dbh, query) != 0) {
		log_event(logger, " SQLERR query: ", query, " failed!\n", NULL);
		free(query);
		return NULL;
	}
/*
 * retrieve sql data from database
 */

	if((res = mysql_store_result(dbh)) == NULL)
		log_event(logger, " SQLERR: empty result\n", NULL);

	free(query);
	return res;
}
static char * getdata(char * buf) {
	char * val;
	char * curr;
	int len = 0;
	int i;

	while(*buf++)
		if(isalnum(*buf) && isspace(*(buf+1))) {
			buf++;
			break;
		}
	curr = ++buf;
	while(*buf++ != '\n')
		len++;
	val = (char *) malloc((len+1) * sizeof(char));
	for(i = 0; i < len; i++)
		val[i] = curr[i];
	val[strlen(val)-1] = '\0';

	return val;
}
