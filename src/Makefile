all:
	gcc -o ddns-server ddserv.c functions.c server.c common.c auth.c -lcrypt
	gcc -o ddns-client client.c common.c
client:
	gcc -o ddns-client client.c common.c
server:
	gcc -o ddns-server ddserv.c functions.c server.c common.c auth.c -lcrypt
clean:
	rm -f ddns-client ddns-server
