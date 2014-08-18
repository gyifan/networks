#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "wrappers.h"

#define MAXLEN    65507

int echo(char * host, int port, char * msg, int msgLen, int source_sd, struct sockaddr_in * cl, int * len);
int initialize(int port);

int main(int argc, char * argv[]){

	int server_port;
	char * dest_host;
	int dest_port;
	char * msg;
	int server_sd;

	int msg_len;
	int * len;                 /* length of received data          */
	struct sockaddr_in * cl;  /* socket addr. structure, client   */

	double p, x; // error generator variables

	/* allocate the parameters */
	len = (int *)wrp_malloc(sizeof(int));
	cl = (struct sockaddr_in *)wrp_malloc(sizeof(struct sockaddr_in));

	if(argc != 5) {
		printf("USAGE: %s [port] [dest-host] [dest-port] [error prob]\n", argv[0]);
		printf("[error prob] - probability of forwarding with error. between 0 and 10\n");
		exit(-1);
	}

	server_port = atoi(argv[1]);
	dest_host = argv[2];
	dest_port = atoi(argv[3]);
	p = ((double)atoi(argv[4]))/10;
	msg = (char *)wrp_malloc(MAXLEN);
	server_sd = initialize(server_port);

	while(1){
		/*
		 * Wait for client message 
		 */
		*len = sizeof(struct sockaddr_in);
		msg_len = recvfrom(server_sd, msg, MAXLEN, 0, (struct sockaddr *)cl, (socklen_t *)len);

		/* Randomly alter the message based on the prob. p */
		x = ((double)rand())/RAND_MAX;
		printf(">> |Gateway|\n");
		printf("   Gateway received the message length: %d\n", msg,msg_len);
		printf("     p = %f, x = %f\n", p, x);
		if(x<p){
			msg[0] = 'X';
			printf("   Forward with ERROR\n");
		}else
			printf("   Forward CORRECTLY\n");
		printf("<< End Gateway\n");
		echo(dest_host, dest_port, msg, msg_len, server_sd, cl, len);
	}

	return 1;
}

int echo(char * host, int port, char * msg, int msgLen, int source_sd, struct sockaddr_in * cl_source, int * len){

	char *retbuf;
	int retbufLen;

	int sd;
	struct addrinfo *ptrh;
	struct sockaddr_in sa;
	struct sockaddr_in cl;

	// Allocates the memory for return buffer.
	retbuf = (char *)wrp_malloc((msgLen+1)*sizeof(char));

	// Allocates an open socket
	sd = wrp_socket(AF_INET, SOCK_DGRAM, 0);

	// Gets server host information
	getaddrinfo(host, NULL, NULL, &ptrh);

	// Sets up socket address data for the server
	memset((char *)&sa, 0, sizeof(sa));
	memcpy(&sa, (struct sockaddr_in *)(ptrh->ai_addr), ptrh->ai_addrlen);
	sa.sin_family   = AF_INET;
	sa.sin_port     = htons(port);

	// Sets up socket address data for the client
	memset((char *)&cl, 0, sizeof(cl));
	cl.sin_family   = AF_INET;
	cl.sin_addr.s_addr = htonl(INADDR_ANY);
	/* port 0 allows the systme to assign any available port */
	cl.sin_port     = htons(0);
	/* if we insist on using the same port number, then the client
	 * and the server have to run on different hosts to avoid port
	 * number conflict
	 */

	// Binds the socket to the client
	wrp_bind(sd, (struct sockaddr *)(&cl), sizeof cl);

	// Sends the message to the server and receive the echo back
	sendto(sd, msg, msgLen, 0, (struct sockaddr *)&sa, sizeof(sa));

	return 1;
}

/* Return the server socket id */
int initialize(int port){

	struct sockaddr_in  sa;  /* socket addr. structure, server   */
	int s;                   /* socket descriptor                */

	/*
	 * Allocate an open socket.
	 */
	s = wrp_socket(AF_INET, SOCK_DGRAM, 0);

	/*
	 * Set up socket address data for the server
	 */
	memset((char *)&sa, 0, sizeof(sa));
	sa.sin_family      = AF_INET;
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	sa.sin_port        = htons(port);

	/*
	 * Bind the socket to the service
	 */
	wrp_bind(s, (struct sockaddr *)(&sa), sizeof sa);

	return s;
}

