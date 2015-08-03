#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <mysql.h>
#include <time.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "clientmanager.h"

static char * getdata(char *);

void InitConnData(REMOTEDATA_t * conn) {
	conn->client_ip_addr = NULL;
	conn->login = NULL;
	conn->pass = NULL;
	conn->subdomain = NULL;
}
void InitDBData(DB_USERDATA_t * db) {
	db->active = 0;
	db->email = NULL;
	db->id = 0;
	db->login = NULL;
	db->md5 = NULL;
	db->subdomain = NULL;
}
int bindToInterface(int portno) {
	int sockfd;
	struct sockaddr_in serv_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0) {
		perror("ERROR opening socket");
		return -1;
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portno);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("ERROR on binding");
		return -1;
	}
	listen(sockfd, 5);

	return sockfd;
}
REMOTEDATA_t * readCLientData(int sockfd, int logger) {
	int clifd;
	REMOTEDATA_t * data;
	struct conn_st * conn_data;

	char * welcome = "dDNS-ng server ready. Go ahead\n";
	char * unknown = "Unknown command\n";
	char * ack = "ACK\n";
	char * val;

	char readbuf[256];

	data = (REMOTEDATA_t *) malloc(sizeof(REMOTEDATA_t));
	bzero(readbuf, 256);

	if((conn_data = clientConn(sockfd)) == NULL) {
		log_event(logger, " ERROR: can't establish connection\n", NULL);
		return NULL;
	}
	data->client_ip_addr = conn_data->client_ip;

	write(conn_data->fd, welcome, strlen(welcome) +1);
	bzero(readbuf, 256);
	while(read(conn_data->fd, readbuf, 256) > 0) {
		val = getdata(readbuf);
		if(strstr(readbuf, "QUIT") != NULL)
			break;
		else if(strstr(readbuf, "LOGIN") != NULL) {
			data->login = (char *) malloc((strlen(val) + 1) * sizeof(char));
			bzero(data->login, sizeof(data->login));
			strcpy(data->login, val);
		}
		else if(strstr(readbuf, "PASS") != NULL) {
			data->pass = (char *) malloc((strlen(val) + 1) * sizeof(char));
			bzero(data->pass, sizeof(data->pass));
			strcpy(data->pass, val);
		}
		else if(strstr(readbuf, "SUBDOMAIN") != NULL) {
			data->subdomain = (char *) malloc((strlen(val) + 1) * sizeof(char));
			bzero(data->subdomain, sizeof(data->subdomain));
			strcpy(data->subdomain, val);
		}
		else {
			write(conn_data->fd, unknown, strlen(unknown)+1);
			continue;
		}
		write(conn_data->fd, ack, strlen(ack)+1);
		free(val);
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
			u.email, u.active, s.dynamic FROM subdomains s, domains d , users u \
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

	if((res = mysql_store_result(dbh)) == NULL || mysql_num_rows(res) == 0)
		return NULL;

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
	val[strlen(val)] = '\0';

	return val;
}
struct subdomain_st * explodeDomain(char * fulldomain) {
	struct subdomain_st * name;
	int sub_len = 0;				// subdomain string lenght
	int dom_len = 0;				// domain string lenght
	char * cur;						// actual position inside the string
	int i;

	cur = fulldomain;
	while(*cur++ != '.')
		sub_len++;
	name->sub = (char *) malloc((sub_len+1) * sizeof(char));
	for(i = 0; i < sub_len; i++)
		name->sub[i] = fulldomain[i];
	name->sub[i] = '\0';

	cur = strchr(fulldomain, '.');
	cur++;
	fulldomain = cur;
	while(*cur++)
		dom_len++;
	name->dom = (char *) malloc((dom_len + 1) * sizeof(char));
	for(i = 0; i < dom_len; i++)
		name->dom[i] = fulldomain[i];
	name->dom[i] = '\0';

	return name;
}
int existZoneFile(char * filepath) {
	FILE * zf;

	if((zf = fopen(filepath, "r")) == NULL)
		return 0;
	else {
		fclose(zf);
		return 1;
	}
}
int existEntry(char * what, char * where) {
	char buf[256];
	FILE * zf;

	if((zf = fopen(where, "r")) == NULL)
		return -1;			// this shouldn't happen
	while(fgets(buf, sizeof(buf), zf) != NULL)
		if(strstr(buf, what) != NULL)
			return 1;		// return success if string found in buffer
	return 0;
}
char * stripSerialNo(char * input) {
	char * serial;
	int i = 0;

	serial = (char *) malloc(13 * sizeof(char));
	while(*input++) {
		if(isdigit(*input)) {
			serial[i] = *input;
			i++;
		}
	}
	serial[i] = '\0';

	return serial;
}
char * newSerialNo(char * serial) {
    time_t today;
    struct tm tf;
    char * newserial;
    char * ver = "00";
    long bigger_serial = 0;

    newserial = (char *) malloc((strlen(serial)+1) * sizeof(char));
    time(&today);
    localtime_r(&today, &tf);
    strftime(newserial, sizeof(newserial), "%Y%m%d", &tf);
    strcat(newserial, ver);

	if(atol(serial) >= atol(newserial)) {
		bigger_serial = atol(serial) + 1;
		sprintf(newserial, "%ld", bigger_serial);
	}

    return newserial;
}
int updateZone(char * subdomain, char * ipaddr, char * file, int logger) {
    FILE *zf;
    FILE *tmp;

    char * serial;
    char * newserial;
    char buf[256];
    char * tmpf;

    tmpf = tempFile(8);
    if((zf = fopen(file, "r+")) == NULL) {
    	log_event(logger, " Error: Unable to open: ", file, "\n", NULL);
        return 0;
    }
    if((tmp = fopen(tmpf, "w+")) == NULL) {
    	log_event(logger, " Error: Unable to open: ", tmpf, "\n", NULL);
    	return 0;
    }

    while(fgets(buf, sizeof(buf), zf) != NULL) {
	    if(strstr(buf, "; serial") != NULL) {
	    	serial = stripSerialNo(buf);
	    	newserial = newSerialNo(serial);
	    	sprintf(buf, "\t%s\t; serial\n", newserial);
	    }
	    if(strstr(buf, subdomain) != NULL) {
		    if(strlen(subdomain) < 8)
		    	sprintf(buf, "%s\t300\tIN\tA\t%s\n", subdomain, ipaddr);
		    else
		    	sprintf(buf, "%s\t300\tIN\tA\t%s\n", subdomain, ipaddr);
	    }
	    fputs(buf, tmp);
    }
    free(serial);
    free(newserial);
    rewind(zf);
    rewind(tmp);
    while(fgets(buf, sizeof(buf), tmp) != NULL)
    	fputs(buf, zf);

    fclose(zf);
    fclose(tmp);
    free(tmpf);
//    apply(tmp, file, cf->domain);

    return 1;
}
char * tempFile(int len) {
    char * entropy = "1qaz2wsx3edc4rfv5tgb6yhn7ujm8i0poklaZAQ1XSW2CDE3VFR4BGT5NHY6MJU6MJU7";
    int n, i;
    struct timeval tv;
    char * tmp_prefix = "/tmp/dyndns_";             // prefix of temp file
    char * tmp_path;                                // full path to tempfile
    char * cur;

    tmp_path = (char *) malloc((strlen(tmp_prefix) + len + 2) * sizeof(char));
    strcpy(tmp_path, tmp_prefix);
    cur = tmp_path+strlen(tmp_prefix);

    for(i=0; i<len; i++, cur++) {                  // while counter is smaller then lenght of random part
            gettimeofday(&tv, NULL);                // get current time
            srand(tv.tv_usec);                      // generate new seed with time in micro seconds
            n = rand() % strlen(entropy);           // generate new random character
            *cur = entropy[n];                     // and put it in the next field of an array.
    }
    *cur = '\0';                                   // end of string containing random filename
    return tmp_path;
}
int dbUpdate(MYSQL * dbh, DB_USERDATA_t * data, struct subdomain_st * domain, char * ipaddr, char * timestamp_s) {
	int len = 0;
	char userid[20];
	char * q_subdomains_1 = "UPDATE subdomains SET ip = '";
	char * q_subdomains_2 = "', lastupdate = '";
	char * q_subdomains_3 = "' WHERE user_id = ";
	char * q_subdomains_4 = " AND subdomain = '";
	char * q_subdomains_5 = "' AND domain_id = (SELECT id FROM domains WHERE domain LIKE '";
	char * q_subdomains_6 = "%')";

	char * q_userlog = "INSERT INTO user_log(user_id,ip,date) VALUES(";
	char * query;

	sprintf(userid, "%d", data->id);
	len = strlen(q_subdomains_1) + strlen(q_subdomains_2) + strlen(q_subdomains_3) + strlen(q_subdomains_4) +
			strlen(q_subdomains_5) + strlen(q_subdomains_6) + strlen(timestamp_s) + strlen(ipaddr) + strlen(userid) +
			strlen(domain->sub) + strlen(domain->dom);
	query = (char *) malloc((len+1) * sizeof(char));

	strcpy(query, q_subdomains_1);
	strcat(query, ipaddr);
	strcat(query, q_subdomains_2);
	strcat(query, timestamp_s);
	strcat(query, q_subdomains_3);
	strcat(query, userid);
	strcat(query, q_subdomains_4);
	strcat(query, domain->sub);
	strcat(query, q_subdomains_5);
	strcat(query, domain->dom);
	strcat(query, q_subdomains_6);

	if(mysql_query(dbh, query) != 0) {
		free(query);
		return 0;
	}
	free(query);

	len = strlen(q_userlog) + strlen(ipaddr) + strlen(timestamp_s) + strlen(userid) + 9;
	query = (char *) malloc((len+1) * sizeof(char));

	strcpy(query, q_userlog);
	strcat(query, userid);
	strcat(query, ", '");
	strcat(query, ipaddr);
	strcat(query, "', '");
	strcat(query, timestamp_s);
	strcat(query, "')");

	if(mysql_query(dbh, query) != 0) {
		free(query);
		return 0;
	}

	free(query);
	return 1;
}
