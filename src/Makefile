all:
	gcc -o ddns-server ddserv.c functions.c server.c common.c auth.c logger.c -lcrypt
	gcc -o ddns-client client.c common.c logger.c
client:
	gcc -o ddns-client client.c common.c logger.c
server:
	gcc -o ddns-server ddserv.c functions.c server.c common.c auth.c logger.c -lcrypt
clean:
	rm -f ddns-client ddns-server
