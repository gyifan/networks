/*
 * CSCI 363 - Computer Networks
 * Lab 3 - TCP in different languages
 * Yifan Ge
 * 02/11/2013
 *
 * File name: pingpong.c
 * Description:
 * 	Send a number of messages of a given size to another program
 * and wait for it to come back. Measure the following times.
 *  1. The time it takes to establish a TCP connection to the server
 *  2. Round trip time (RTT)
 *  3. The time is takes to tear down an existing TCP connection.
 *  4. The total time for establishing the connection.
 *  
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

#include "wrappers.h"
#include "tcplib.h"
#include "list.h"

#define BUFSIZE 256

int sendList(char *host, int port, int num, struct list_t *list){

	int sd;
	struct node_t *node;	// node to store the sending node.
	char buf[15];

	// Establish the TCP socket connection
	sd = socketClient(host, port);
	if(sd < 0){
		switch(sd){
			case(-1): 
				printf("Remote host does not exist\n");
				break;
			case(-2): 
				printf("Socket creation failed\n");
				break;
			case(-3): 
				printf("Remote host connection failed\n");
				break;
			default: printf("Unknown Error. socketClient failed\n");
		}
		exit(-1);
	}

	// Send the number of nodes to the server.
	sprintf(buf, "%d", num);
	wrp_writen(sd, buf, 15);

	// Send the node to the server.
	while(NULL != (node = remove_head(list))){
		wrp_writen(sd, node, sizeof(struct node_t));
		printf("sending...\n");
		printf(">> %s\n", node->words);
		free(node);
	}
	free(list);

	close(sd);

	return 1;
}

int main(int argc, char* argv[]){

/*
	int msgNum;		// Number of messages need to send.
	int msgLen;		// Length of messages need to send.
	char *msg;		// Message pointer. Space will be dynamically allocated.

*/
	int num;		// number of nodes
	char *host;		// host name;
	int port;		// Port number;
	FILE *fd;			// file descriptor with messages

	struct list_t *list;	// List of nodes
	struct node_t *tmpNode;	// temp node
	char *buf;		// buf to store the messages

	int retval;
	char *retfgets;

	if(argc != 4){
		printf("Usage: pingpong [host] [port] [filename]\n");
		exit(-1);
	}

	// Fetch the information from the input.
	host = argv[1];
	port = atoi(argv[2]);
	fd = wrp_fopen(argv[3], "r");
	
	// Makes the list with the messages in the file
	list = (struct list_t *)wrp_malloc(sizeof(struct list_t));
	num = 0;
	do{
		buf = (char *)wrp_malloc(BUFSIZE);
		retfgets = fgets(buf, BUFSIZE, fd);
		if(strlen(buf) == 0);
		else{
			num++;
			tmpNode = make_node(buf, num, 0);
			insert(list, tmpNode);
		}
	}while(retfgets != NULL);

	// Send the message to the server and accumulate the timing stats.
	retval = sendList(host, port, num, list);
	if(retval != 1){
		printf("tcpAnalyzer failed\n");
		exit(-1);
	}
	return 1;
}
