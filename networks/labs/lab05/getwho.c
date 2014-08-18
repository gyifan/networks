/*
 * getwho_client.c - main 
 */

/*
 * The getwho client connects to a sever, sends a request for user list
 * and wait the string being sent back. Then it prints the list to the
 * stdout upon receive the list.
 *
 */
#include <stdio.h>
#include <string.h>
#include "tcplib.h"
#include "wrappers.h"

#define GETWHO_PORT  14117 /* a non-privilege port so all can use */
#define BUFFSIZE     1024

int main(int argc, char *argv[])
{
	int s;                   /* socket descriptor                */
	int len;                 /* length of received data          */
	char buf[BUFFSIZE+1];    /* buffer to read echo info         */
	
	int usersize;			 /* user string size read from server */

	if (argc != 2)   {
		fprintf(stderr,"usage %s hostname\n",argv[0]);
		exit(1);
	}
	s = socketClient(argv[1], GETWHO_PORT);
	/* Send the message  */
	if (write(s, buf, len) != len) {
		perror("write");
		exit(1);
	}

	/*
	 * Read the reply and put to user's output
	 */
	strcpy(buf, "");	
	len = wrp_readn(s, buf, 80);
	if(len < 0){
		printf("Error reading from socket\n");
		exit(-1);
	}
	buf[len] = 0;
	usersize = atoi(buf);
	strcpy(buf, "");

	len = wrp_readn(s, buf, usersize);
	if(len < 0){
		printf("Error reading from socket\n");
		exit(-1);
	}
	printf("%s", buf);
	close(s);

	return 0;
}
