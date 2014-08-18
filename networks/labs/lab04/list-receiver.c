/*
 * CSCI 363 - Computer Networks
 * Lab 4 - TCP w/ sending structs
 * Yifan Ge
 * 02/18/2013
 *
 * File name: list-receiver.c
 * Description:
 *	This program will be a daemon, waiting for request from client.
 *
 */

#include <stdio.h>
#include <unistd.h>
#include "wrappers.h"
#include "tcplib.h"

#include "list.h"

#define BUFSIZE 256

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

	struct node_t * rcvNode;
	struct list_t * rcvList;

	int i;
	
	rcvList = (struct list_t *)wrp_malloc(sizeof(struct list_t));
	n = wrp_readn(t, sizebuf, 15);
	sizebuf[n] = 0;
	size = atoi(sizebuf);

	for(i=0; i<size; i++){
		buf = (char *)wrp_malloc(sizeof(struct node_t));	
		n = wrp_readn(t, buf, sizeof(struct node_t));
		rcvNode = (struct Node *)buf;
		insert(rcvList, rcvNode);
	}

	print_list(rcvList);

	close(t);
	free(buf);
}
