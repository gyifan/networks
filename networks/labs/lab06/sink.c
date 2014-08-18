/*
 * CSCI 363 - Computer Networks
 * Lab 06 - UDP
 * 03/16/2014
 * Yifan Ge
 *
 * Credit: This program is based on Prof. Xianlong Meng's class example.
 * 	udp-server.c
 *
 */

/*
 * A simple UDP echo server
 */
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "wrappers.h"

#define MAXLEN    1000000

int main(int argc, char*argv[]) {

	int s;                   /* socket descriptor                */
	int len;                 /* length of received data          */
	struct sockaddr_in  sa;  /* socket addr. structure, server   */
	struct sockaddr_in  cl;  /* socket addr. structure, client   */
	char msg[MAXLEN];
	int  msg_len;

	int port;

	if(argc != 2) {
		printf("USAGE: reflectord [port]\n");
		exit(-1);
	} 
	
	port = atoi(argv[1]);

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

	/*
	 * Wait for client message and echo the message back
	 */
	while (1) {
		len = sizeof(struct sockaddr_in);
		msg_len = recvfrom(s, msg, MAXLEN, 0, (struct sockaddr *)&cl, (socklen_t *)&len);
		msg[msg_len] = 0; // terminate the buffer
		printf("Sink received: %s Length: %d\n",msg, msg_len);
		fflush(stdout);
		sendto(s, msg, msg_len, 0, (struct sockaddr *)&cl, len);


	}

	return 0;
}

