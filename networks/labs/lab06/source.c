/*
 * CSCI - Comptuer Networks
 * Lab 04 - UDP
 * 02/17/2014
 * Yifan Ge
 *
 * Credit: This program is based on Prof. Xianlong Meng's class example:
 * 	udp-client.c
 *
 */

#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>

#include "wrappers.h"

double gettime(){
	struct timeval tv;

	wrp_gettimeofday(&tv, NULL);
		
	return tv.tv_usec+tv.tv_sec*1000000;
}

int main(int argc, char* argv[]){
	
	int msgLen;
	char *host;
	int port;

	char *msg;
	char *retbuf;
	int retbufLen;

	int sd;
	struct addrinfo *ptrh;
	struct sockaddr_in sa;
	struct sockaddr_in cl;

	int i; // counter

	if(argc != 4){
		printf("USAGE: source [host] [port] [msg length]\n");
		exit(-1);
	}
	
	// Gets parameters from input
	msgLen = atoi(argv[3]);
	host = argv[1];
	port = atoi(argv[2]);

	// Makes the message based on the length of the message.
	msg = (char *)wrp_malloc((msgLen+1)*sizeof(char));
	for(i=0; i<msgLen; i++)
		msg[i] = 'M';
	msg[msgLen] = '\0'; // insert end of string char.
	
	// Allocates the memory for return buffer.
	retbuf = (char *)wrp_malloc((msgLen+1)*sizeof(char));
	
	// Allocates an open socket
	sd = wrp_socket(AF_INET, SOCK_DGRAM, 0);

	// Gets server host information
	getaddrinfo(host, NULL, NULL, &ptrh);

	// Sets up socket address data for the server
	memset((char *)&sa, 0, sizeof(sa));
	memcpy(&sa, (struct sockaddr_in *)(ptrh->ai_addr), ptrh->ai_addrlen);
	sa.sin_family	= AF_INET;
	sa.sin_port	= htons(port);

	// Sets up socket address data for the client
	memset((char *)&cl, 0, sizeof(cl));
	cl.sin_family	= AF_INET;
	cl.sin_addr.s_addr = htonl(INADDR_ANY);
	/* port 0 allows the systme to assign any available port */
	cl.sin_port	= htons(0);
	/* if we insist on using the same port number, then the client
 	 * and the server have to run on different hosts to avoid port
 	 * number conflict
 	 */

	// Binds the socket to the client
	wrp_bind(sd, (struct sockaddr *)(&cl), sizeof cl);
	printf("Source sending %s ... ... Length: %d\n", msg, msgLen);
	// Sends the message to the server and receive the echo back
	sendto(sd, msg, msgLen, 0, (struct sockaddr *)&sa, sizeof(sa));
	retbufLen = recvfrom(sd, retbuf, msgLen, 0, NULL, NULL);
	retbuf[retbufLen] = 0;
	if(0 == strncmp(msg, retbuf, msgLen))
		printf("matching echo\n");
	else{
		printf("unmatching echo\n");
		printf("Original msg: %s \nReceived msg: %s\n", msg, retbuf);
	}
	return 1;
}
