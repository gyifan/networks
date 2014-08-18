/*
 * Copyright (c) 2012 Bucknell University
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: L. Felipe Perrone (perrone@bucknell.edu)
 */

/*
 *  Revision:
 *  Xiannong Meng 01-07-2013
 *  -- Change the use of 'gethostbyname' to 'getaddrinfo'
 */ 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "wrappers.h"
#include "writen.c"
#include "readn.c"

extern int errno;

/*------------------------------------------------------------------------
 * Program:   getfile 
 *
 * Purpose:   allocate a socket, connect to a server, transfer requested
 *            file to local host, and print file contents to stdout
 *
 * Syntax:    getfile [ host ] [ port ] [ file ] 
 *
 *		 host  - name of a computer on which server is executing
 *		 port  - protocol port number server is using
 *               file  - complete path of file to retrieve
 *
 *------------------------------------------------------------------------
 */

int main(int argc, char* argv[]) {

	struct addrinfo *ptrh; // pointer to a host table entry	
	struct protoent *ptrp; // pointer to a protocol table entry	
	struct sockaddr_in sad; // structure to hold an IP address	
	int   status;          // calling status
	int	sd;		 // socket descriptor			
	int	port;		 // protocol port number		
	char	*host;		 // pointer to host name		
	char	buf[1000];	 // buffer for data from the server	
	char  file_name[257];  // buffer for full path to file
	int   bytes_read;      // number of bytes read from socket

	// Check command-line arguments
	if (argc < 4) {
		printf("usage: getfile [ host ] [ port ] [ file ]\n");
		exit(-1);
	}

	host = argv[1];		
	port = atoi(argv[2]);	
	strncpy(file_name, argv[3], 257);
	file_name[strlen(argv[3])] = 0;  // terminate the string

	if (port <= 0)  {
		// test for legal value		
		// print error message and exit	
		fprintf(stderr,"GETFILE: bad port number %s\n",argv[2]);
		exit(1);
	}

	// Convert host name to equivalent IP address and copy to 'sad'. 
	status = getaddrinfo(host, NULL, NULL, &ptrh);
	if ( status != 0 ) {
		fprintf(stderr,"GETFILE: invalid host: %s\n", host);
		exit(1);
	}

	memset((char *)&sad,0,sizeof(sad)); // clear sockaddr structure	
	memcpy(&sad, (struct sockaddr_in *)(ptrh->ai_addr), ptrh->ai_addrlen);
	sad.sin_port = htons((u_short)port);

	// Map TCP transport protocol name to protocol number. 
	ptrp = getprotobyname("tcp");
	if ( ptrp == NULL ) {
		fprintf(stderr, "GETFILE: cannot map \"tcp\" to protocol number");
		exit(1);
	}

	// Create a socket. 
	sd = wrp_socket(PF_INET, SOCK_STREAM, ptrp->p_proto);

	// Connect the socket to the specified server.
	wrp_connect(sd, (struct sockaddr *)&sad, sizeof(sad));

	// Repeatedly read data from socket and write to user's screen. 
	strncpy(buf, "file", 5);
	wrp_writen(sd, buf, 5);

	strncpy(buf, file_name, strlen(file_name)+1);
	buf[strlen(file_name) + 1] = 0; // terminate the buffer
	wrp_writen(sd, buf, strlen(file_name)+1);

	do {
		bytes_read = wrp_readn(sd, buf, 1);
		if (bytes_read > 0)
			printf("%c", buf[0]);
	} while (bytes_read > 0);

	// Close the socket.   
	close(sd);

	// Terminate the client program gracefully. 
	exit(0);
}
