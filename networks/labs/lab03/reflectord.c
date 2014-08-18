/*
 * CSCI 363 - Computer Networks
 * Lab 3 - TCP in different languages
 * Yifan Ge
 * 02/11/2013
 *
 * File name: reflectord.c
 * Description:
 *	This program will be a daemon, waiting for messages and return
 * the same message to the client.
 *
 */

#include <stdio.h>
#include <unistd.h>
#include "wrappers.h"
#include "tcplib.h"


int main(int argc,char *argv[])    {

	int s, t;
	int port;
	void doEcho(int);

	if(argc != 2){
		printf("Usage: reflectord [port]\n");
		exit(-1);
	}

	port = atoi(argv[1]);

	s = socketServer(port);
	printf("orig socket %d\n", s);
	while (1) {
		if (s > 0)
			t = acceptConn(s);
		else {
			fprintf(stderr," socket error\n");
			exit(1);
		}
		if (t > 0)  {
			doEcho(t);
		}  else {
			fprintf(stderr," connection error\n");
			exit(2);
		}
	} /* while(1) */
	close(s);
	return 0;
}

void doEcho(int t)   {

	char *buf;
	char sizebuf[15];
	int n;
	int size;

	n = wrp_readn(t, sizebuf, 15);
	sizebuf[n] = 0;
	size = atoi(sizebuf);

	buf = malloc((size+2)*1);
	buf[size+1] = 0;	

	n = wrp_readn(t, buf, size+1);
	buf[n] = 0;
	wrp_writen(t, buf, size+1);
	close(t);
	free(buf);
}
