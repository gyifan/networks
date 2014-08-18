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

double gettime(){
	struct timeval tv;

	if( 0 > gettimeofday(&tv, NULL)){
		printf("error gettimeofday\n");
		return -1;
	}

	return tv.tv_usec+tv.tv_sec*1000000;
}

int tcpAnalyzer(char *host, int port, char *msg, double *est_t, double *rtt, double *close_t, double *total_t){

	int sd;
	int msgLen;
	char *buf;		// buffer to store the sending message.
	char *retbuf;	// buffer to store the return message.

	double tmptime;
	double starttime;

	int retval;

	starttime = gettime(); // record the starting time.

	msgLen = strlen(msg);
	buf = (char *)malloc((msgLen+2)*sizeof(char));
	retbuf = (char *)malloc((msgLen+2)*sizeof(char));

	tmptime = gettime();
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
	*est_t = gettime() - tmptime; // Get establish time

	// Send the message length to the server.
	sprintf(buf, "%d", msgLen);
	wrp_writen(sd, buf, 15);

	// Copy the message to the send buffer.
	strncpy(buf, msg, msgLen+1);
	buf[msgLen+1] = 0; // terminate the buffer

	tmptime = gettime();
	// Send the message to the server.
	wrp_writen(sd, buf, msgLen+1);

	// Read the return message from the server.
	retval = wrp_readn(sd, retbuf, msgLen+1);
	if(retval != msgLen+1)
		printf("The message didn't return correctly %d, correct size %d\n", retval, msgLen+1);
	*rtt = gettime() - tmptime; // Get RTT

	tmptime = gettime();
	close(sd);
	*close_t = gettime() - tmptime;	

	*total_t = gettime() - starttime; // Get total time
	free(buf);
	free(retbuf);
	return 1;
}

int main(int argc, char* argv[]){

	int msgNum;		// Number of messages need to send.
	int msgLen;		// Length of messages need to send.
	char *msg;		// Message pointer. Space will be dynamically allocated.
	char *host;		// host name;
	int port;		// Port number;

	// timing variables
	double est_temp;
	double rtt_temp;
	double close_temp;
	double total_temp;

	double est_t = 0;
	double rtt_t = 0;
	double close_t = 0;
	double total_t = 0;

	int i;
	int retval;

	if(argc != 5){
		printf("Usage: pingpong [num_msgs] [msg_length] [host] [port]\n");
		exit(-1);
	}

	// Fetch the information from the input.
	msgNum = atoi(argv[1]);
	msgLen = atoi(argv[2]);
	host = argv[3];
	port = atoi(argv[4]);

	// Allocate the message with the desired length
	msg = (char*)malloc((msgLen+1)*sizeof(char));
	if(msg == NULL){
		printf("malloc failed\n");
		exit(-1);
	}
	for(i=0; i<msgLen; i++)
		msg[i] = 'M';
	msg[msgLen] = '\0'; // insert end of string char;

	// Send the message to the server and accumulate the timing stats.
	for(i=0; i<msgNum; i++){
		retval = tcpAnalyzer(host, port, msg, &est_temp, &rtt_temp, &close_temp, &total_temp);
		if(retval != 1){
			printf("tcpAnalyzer failed\n");
			exit(-1);
		}

		est_t+=est_temp;
		rtt_t+=rtt_temp;
		close_t+=close_temp;
		total_t+=total_temp;
	}

	// Calculate the average timing stats.
	est_t = est_t/msgNum/1000;
	rtt_t = rtt_t/msgNum/1000;
	close_t = close_t/msgNum/1000;
	total_t = total_t/msgNum/1000;

	printf("%f %f %f %f\n", est_t, rtt_t, close_t, total_t);

	return 1;
}
