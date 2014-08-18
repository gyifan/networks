/*
 * The program listens on multiple ports for client requests.
 * The library call 'select()' is used to detect if any requests are received
 * at any of the ports.
 *
 * For each request that is detected, the server spaws a process to handle the
 * request.
 *
 */
#include <stdio.h>
#include <string.h>
#include "tcplib.h"

#include <utmp.h>
#include <time.h>
#include "wrappers.h"

#define TIMEPORT 14017
#define ECHO_PORT 14018
#define GETWHO_PORT 14117
#define USERSIZE 1024
#define BUFSIZE 512


/* services supported */
void echoService(int);
void getwhoService(int);
void timeService(int);
int  socketUDPServer(int);

int main(int argc, char *argv[])   {

	int timeSock, echoSock;   /* sockets on which the server listens */
	int timeWork, echoWork;   /* sockets where the service is provided */
	int getwhoSock, getwhoWork; /* sockets for getwho service */ 
	int nfound;               /* number of connection requests found */
	int maxfdp1;              /* maximum number the server need to check */

	struct sockaddr_in  sa;  /* socket addr. structure, server   */
	struct sockaddr_in  cl;  /* socket addr. structure, client   */

	fd_set readmask;          /* file descriptor mask */

	/* create two sockets */
	timeSock = socketUDPServer(TIMEPORT);
	echoSock = socketServer(ECHO_PORT);
	getwhoSock = socketServer(GETWHO_PORT);

	/* initialize the readmask to be all zero */
	FD_ZERO(&readmask);

	/* servers' main loop */
	for (; ;)    {

		FD_SET(timeSock, &readmask);   /* set timeSock in mask */
		FD_SET(echoSock, &readmask);   /* set echoSock in mask */
		FD_SET(getwhoSock, &readmask); /* set getwhoSock in mask */

		maxfdp1 = getwhoSock + 1;

		/* check how many are ready */
		nfound = select(maxfdp1, &readmask, (fd_set *)0, (fd_set*)0,
				(struct timeval *)0);

		if (nfound < 0)  {
			perror("select error");
			exit(3);
		}

		/* check which socket is read */
		int timeReady = FD_ISSET(timeSock, &readmask);
		int echoReady = FD_ISSET(echoSock, &readmask);
		int getwhoReady = FD_ISSET(getwhoSock, &readmask);

		if (timeReady)
			timeWork = timeSock;
		if (echoReady)
			echoWork = acceptConn(echoSock);
		if (getwhoReady)
			getwhoWork = acceptConn(getwhoSock);

		int pid = fork();
		if (pid == 0) { // child
			if (timeReady) {
				printf("time service requested\n");
				fflush(stdout);
				timeService(timeWork);
				close(timeWork);
			}

			if (echoReady) {
				printf("echo service requested\n");
				fflush(stdout);
				echoService(echoWork);
				shutdown(echoWork, 2);
			}

			if (getwhoReady) {
				printf("getwho service requested\n");
				fflush(stdout);
				getwhoService(getwhoWork);
				close(getwhoWork);
			}
			exit(0);
		} else { // end of child process
			//      wait(NULL); // so the server exits gracefully
		}
	}  // end of server loop
}  // end of program

void getwhoService(int sock){
	
	char buf[80];
	char ret_buf[USERSIZE];

    int count = 0;
    struct utmp * one_user;
    setutent();
	
	strcpy(ret_buf, "Username list: \n");
   
	one_user = getutent();
	
    while (one_user != NULL) {
        if(one_user->ut_type == USER_PROCESS){
            sprintf(buf, "username: %s\n", one_user->ut_user);
            printf("username: %s\n", one_user->ut_user);
			strcat(ret_buf, buf);
		}
        count ++;
        one_user = getutent();
    }   
    endutent();
    printf("a total of %d entries was encounted\n", count);	

	sprintf(buf, "%d", strlen(ret_buf));
	buf[strlen(buf)] = 0;

	wrp_writen(sock, buf, 80);
	wrp_writen(sock, ret_buf, strlen(ret_buf));
	
}

#include <sys/time.h>

/* time service code */
void timeService(int sock) {

	struct sockaddr_in cl;
	int len = sizeof(struct sockaddr_in);

	int request;
	int bytes_received = recvfrom(sock, &request, sizeof(int), 
			0, (struct sockaddr *)&cl, &len);

	printf("bytes received %d\n", bytes_received);

	struct timeval time;
	gettimeofday(&time, NULL);

	/**
	  printf("%s", ctime((time_t *)&(time.tv_sec)));// ctime() converts UNIX
	  printf("%u\n",time.tv_sec);
	 **/
	long n = time.tv_sec + 2208988800; // make it since 1/1/1900

	n = htonl(n);

	sendto(sock, &n, sizeof(long), 0, (struct sockaddr *)&cl, len);
}

/* echo service code */
void echoService(int sock)  {

	char echo_buf[BUFSIZ+10];
	char buf[BUFSIZ+1];
	strcpy(echo_buf, "Echo: ");
	int  n = read(sock, buf, BUFSIZ);
	buf[n] = 0;
	strcat(echo_buf, buf);
	write(sock, echo_buf, strlen(echo_buf));
}

/*
 * Create a UDP server socket and get it ready
 * for receiving.
 * 
 * parameters:
 *    int port: port number
 * 
 * return:
 *    int socket that is ready to receive data
 */
int socketUDPServer(int port) {

	int s;                   /* socket descriptor                */
	int len;                 
	struct sockaddr_in sa;

	/*
	 * Allocate an open socket.
	 */
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("UDP socket error.");
		exit(1);
	}

	/*
	 * Set up socket address data for the server
	 */
	memset((char *)&sa, 0, sizeof(struct sockaddr_in));
	sa.sin_family      = AF_INET;
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	sa.sin_port        = htons(TIMEPORT);

	/*
	 * Bind the socket to the service
	 */
	if (bind(s, (struct sockaddr *)(&sa), sizeof(struct sockaddr_in)) < 0) {
		perror("UDP bind error.");
		exit(1);
	}

	return s;
}
