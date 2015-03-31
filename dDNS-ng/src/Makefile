all:
	gcc -o ddns-server ddserv.c functions.c server.c common.c auth.c -lcrypt -lmysqlclient -L/usr/lib/x86_64-linux-gnu -I/usr/include/mysql
	gcc -o ddns-client client.c common.c
client:
	gcc -o ddns-client client.c common.c
server:
	gcc -o ddns-server ddserv.c functions.c server.c common.c auth.c -lcrypt -lmysqlclient -L/usr/lib/x86_64-linux-gnu -I/usr/include/mysql
clean:
	rm -f ddns-client ddns-server
