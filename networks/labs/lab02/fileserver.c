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

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#include <sys/wait.h>
#include "wrappers.h"

#include "readn.c"

#define MAX_FILE_PATH   257 // length of path+filename and null char  
#define BUFFER_SIZE     512 // size of file read buffer    
#define	QLEN		6   // size of request queue
int	visits	    =   0;  // counts client connections

/*------------------------------------------------------------------------
 * Program:   fileserver
 *
 * Purpose:   allocate a socket and then repeatedly execute the following:
 *		(1) wait for the next connection from a client
 *		(2) send a short message to the client
 *		(3) close the connection
 *		(4) go back to step (1)
 *
 * Syntax:    fileserver [ port ]
 *
 *		 port  - protocol port number to use
 *
 *------------------------------------------------------------------------
 */

void process_request(int fd){

	int n;                         // byte count
	char buf[BUFFER_SIZE];         // buffer for file chunk server sends	
	char reqbuf[MAX_FILE_PATH];

	int file_req;
	int bytes_read;

	// Read request for fileserver
	n = wrp_readn(fd, reqbuf, 5);
	reqbuf[n] = 0;   // terminate the string

	if (strncmp(reqbuf, "file", 5) == 0) {
		// Request is get file, so now read full path to file
		// Pre-condition: full path string is no more than 256 chars long
		n = read(fd, reqbuf, 257);
		reqbuf[n] = 0;
		printf("SERVER: file requested is %s\n", reqbuf);
		fflush(stdout);

		if ((file_req = open(reqbuf,O_RDONLY)) == -1) {
			printf("SERVER: file not found\n");
			fflush(stdout);
		} else {
			printf("SERVER: file %s found\n", reqbuf);
			fflush(stdout);

			do {
				bytes_read = read(file_req, buf, BUFFER_SIZE);
				send(fd, buf, bytes_read, 0);
			} while (bytes_read == BUFFER_SIZE); // Fixed the problem 2
		}
	} else if (strncmp(reqbuf, "stat", 5) == 0) {
		// Request is "get status"
		printf("SERVER: status requested\n");

		sprintf(buf,"Server has been contacted %d time%s\n",
				visits,visits==1?".":"s.");

		send(fd,buf,strlen(buf),0);
	} else {
		// Request is invalid
		printf("SERVER: invalid request\n");
	}
	close(fd);
}

int main(int argc, char* argv[]) {

	struct protoent *ptrp;	 // pointer to a protocol table entry	
	struct sockaddr_in sad;        // structure to hold server's address	
	struct sockaddr_in cad;        // structure to hold client's address	
	int sd, sd2;	                 // socket descriptors			
	int port;		         // protocol port number		
	int alen;		         // length of address			

	int pid;
	int max_active = 3;
	int number_active = 0;
	int status;

	// Check command-line argument for protocol port.

	if (argc > 1) {			
		port = atoi(argv[1]);	        
	}
	else {
		printf("Usage: fileserver [ port ]\n");
		exit(-1);
	}

	if (port <= 0) {
		// test for illegal value	
		// print error message and exit	
		fprintf(stderr,"SERVER: bad port number %s\n",argv[1]);
		exit(1);
	}

	memset((char *)&sad,0,sizeof(sad)); // clear sockaddr structure	
	sad.sin_port = htons((u_short)port);
	sad.sin_family = AF_INET;	      // set family to Internet	        
	sad.sin_addr.s_addr = INADDR_ANY;   // set the local IP address	

	// Map TCP transport protocol name to protocol number 
	//    --> Look at /etc/protocols
	ptrp = getprotobyname("tcp");
	if ( ptrp == NULL) {
		fprintf(stderr, "SERVER: cannot map \"tcp\" to protocol number");
		exit(1);
	}

	// Create a socket 
	sd = wrp_socket(PF_INET, SOCK_STREAM, ptrp->p_proto);

	// Bind a local address to the socket 
	wrp_bind(sd, (struct sockaddr *)&sad, sizeof(sad));

	// Specify size of request queue 
	wrp_listen(sd, QLEN);

	// Main server loop - accept and handle requests 
	// http://stackoverflow.com/questions/12591540/waitpid-and-fork-to-limit-number-of-child-processes

	while(1){
		alen = sizeof(cad);
		sd2=wrp_accept(sd, (struct sockaddr *)&cad, (socklen_t*)(&alen));

		visits++;

		while(number_active >= max_active){
			wait(&status);
			number_active--;
		}
		pid = fork();
		if (pid < 0)
			perror("Failed to fork child");
		if(0 == pid){
			process_request(sd2);
			break;
		}
		else{
			number_active++;
			close(sd2);
		}
	}
	return 1;
}
