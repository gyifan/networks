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

#include <sys/types.h>
#include <sys/stat.h>

#include "wrappers.h"
#include "crc-api.h"

#define MAXFILESIZE 65507
#define CRCSIZE 2

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
/*
	if(argc != 4){
		printf("USAGE: source [gateway host name] [port number] [message length]\n");
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
*/
	// Check if command is correct
	if(argc != 4){
		printf("USAGE: source [gateway host name] [port number] [filename]\n");
		exit(-1);
	}
	
	// Gets parameters from input
	host = argv[1];
	port = atoi(argv[2]);

	/*
	 * Open the file and call fstat
	 * 	int fstat(int fd, struct stat *buf);
	 * fstat returns 0 on success. The file stats is returned in struct ptr
	 *
	 */
	int fd;
	struct stat buf;
	if(-1 == (fd = open(argv[3], O_RDONLY))){
		printf("Error openning the file\n");
		exit(-1);
	}
	if(0 != fstat(fd, &buf)){
		printf("Error fstat the file\n");
		exit(-1);
	}
	
	// Check the file size if can fit in UDP length 65,507 bytes
	msgLen = buf.st_size;
//	printf("file size is %d\n", msgLen);
	if(msgLen > MAXFILESIZE){
		printf("File is too large to send\n");
		exit(-1);
	}

	// Read the whole file to msg
	msg = (char *)wrp_malloc((msgLen+2)*sizeof(char));
	wrp_readn(fd, msg, msgLen);

	/* Add the crc code to msg, increase the msgLen */
	unsigned short crc;
	crc = generate_crc(msg, msgLen);
	memcpy(&(msg[msgLen]),&crc,CRCSIZE);
	msgLen+=2;

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
	printf("\n==========================\n");
	printf("  Starting a new session.\n");
	printf("==========================\n");
	printf(">> |Source|\n");
	printf("   Sending file %s\n", argv[3]);
	printf("   Length: %d\n", msgLen);
	printf("<< End Source\n");
	// Sends the message to the server and receive the echo back
	sendto(sd, msg, msgLen, 0, (struct sockaddr *)&sa, sizeof(sa));
	free(msg);	
	return 1;
}
