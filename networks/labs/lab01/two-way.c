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

/****************************************************************
 * Note that throughout this program, the return value of EVERY  *
 * system call is checked. You will be expected to do the same   *
 * in EVERY program you write in this class, be it for a lab     *
 * or a homework assignment.                                     *
 ****************************************************************/

#include <sys/types.h>
#include <unistd.h> 
#include <string.h>
#include <sys/time.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <sys/time.h>
#include <signal.h>

#define FOREVER 1
#define INPUT   0
#define OUTPUT  1

/**************************************************************

Source: a process that pushes a null-terminated string of 
characters into a pipe.

 ***************************************************************/

// Signal handler for delay
void handler(int sig){

}

void one(int* fd, int* fd_ack)
{
	int retval;
	char  one[80];
	char  two[80];
	int   count = 0;
	int   length;
	char* buffer;

	struct timeval tv;
	char  line[80];

	/* Sets the hold timeval*/
	struct itimerval delay;
	delay.it_value.tv_sec = 0;
	delay.it_value.tv_usec = 500000;
	delay.it_interval.tv_sec = 0;
	delay.it_interval.tv_usec = 500000;

	/* Declare the SIGALRM handler */
	struct sigaction act;
	act.sa_handler = handler;

	strcpy(one, "baseball is boring");
	strcpy(two, "soccer rules");
	buffer = one;
	length = strlen(one);

	printf("Source running...\n");

	// close read end of the pipe
	retval = close(fd[INPUT]);
	if (-1 == retval)
		perror("Error closing input pipe");

#ifdef DEBUG
	printf("DEBUG_one: fd[INPUT] closed\n");
#endif

	// close write end of the ack_pipe
	retval = close(fd_ack[OUTPUT]);
	if (-1 == retval)
		perror("Error closing output ack_pipe");

#ifdef DEBUG
	printf("DEBUG_one: fd_ack[OUTPUT] closed\n");
#endif


	while(FOREVER) {

		/* 
		 * PRINT TIME WHEN START SENDING MESSAGE 
		 */
		retval = gettimeofday(&tv, NULL);
		if (-1 == retval){
			printf("Failed to get the time of the day\n");
			exit(-1);
		}
		printf("\n==========================================================\n");
		printf("P1 starts sending \"%s\" at %s", buffer, ctime(&tv.tv_sec));

		/* 
		 * 
		 * Sending message through the fd pipe to P2
		 *
		 */
		while (count <= length){
			retval = write(fd[OUTPUT], &(buffer[count]), 1);
			if (-1 == retval)
				perror("Error writing to pipe");
#ifdef DEBUG
			printf("DEBUG_one: sent %d byte/s, %c \n", retval, buffer[count]);
#endif
			count++;
		}

		count = 0;

		/* 
		 *
		 * Read response message through the fd_ack pipe from P1 
		 *
		 */
		retval = read(fd_ack[INPUT], &(line[count]), 1);
		if (-1 == retval)
			perror("Error reading from pipe");
#ifdef DEBUG
		printf("DEBUG_one: read %d byte/s, %c \n", retval, line[count]);
#endif
		while (0 != line[count]){
			count++;
			retval = read(fd_ack[INPUT], &(line[count]), 1);
			if (-1 == retval)
				perror("Error reading from pipe");
#ifdef DEBUG
			printf("DEBUG_one: read %d byte/s, %c \n", retval, line[count]);
#endif
		}

		/* 
		 * PRINT TIME WHEN RECIEVED RESPONSE MESSAGE 
		 */
		retval = gettimeofday(&tv, NULL);
		if (-1 == retval){
			printf("Failed to get the time of the day\n");
			exit(-1);
		}
		printf("P1 recieved \"%s\" at %s", line, ctime(&tv.tv_sec));
		printf("==========================================================\n");

		count = 0;

		/* 
		 * Checking line with "ack"
		 */ 
		if (0 == strcmp(line, "ack")){
			// change buffer to the alternative string.
			if (buffer == one)  
				buffer = two;
			else
				buffer = one;

			length = strlen(buffer);
		}

		/* SOURCE DELAY */    
		retval = setitimer(ITIMER_REAL, &delay, NULL);
		if (-1 == retval){
			perror("Set timer ERROR");
			exit(-1);
		}

		sigaction(SIGALRM, &act, NULL);

		pause();
	} 
}


/**************************************************************

Sink: a process that reads characters from a pipe one at a
time and displays them on the screen.

 ***************************************************************/

void two(int* fd, int* fd_ack)
{ 
	char line[80];
	int count;
	int retval;
	struct timeval tv;
	time_t time;
	char ack[80];

	strcpy(ack, "ack");
	printf("Sink running...\n");

	count = 0;

	// close write end of the pipe
	retval = close(fd[OUTPUT]);
	if (-1 == retval)
		perror("Error closing output pipe");

	// close read end of the ack_pipe
	retval = close(fd_ack[INPUT]);
	if (-1 == retval)
		perror("Error closing input ack_pipe");

	while(FOREVER) {
		// read one byte from pipe and store it in buffer
		retval = read(fd[INPUT], &(line[count]), 1);
		if (-1 == retval)
			perror("Error reading from pipe");

		if (0 == line[count]) {
			printf("line = %s\n", line);
			count = 0;

			/* PRINT TIME WHEN FULL MESSAGE RECEIVED */
			retval = gettimeofday(&tv, NULL);
			if (-1 == retval){
				printf("Failed to get the time of the day\n");
				exit(-1);
			}
			time = tv.tv_sec;
			printf("P2 recieved message at %s", ctime(&time));

			/* Write acknowledgement to one */
			while (count <= 3){
				retval = write(fd_ack[OUTPUT], &(ack[count]), 1);
				if (-1 == retval)
					perror("Error writing to pipe");

				count++;
			}
			count = 0;

		}
		else 
			count++; 
	}
}


/**************************************************************

main: creates a pipe and two processes (one and two).

 ***************************************************************/

int main(int argc, char* argv[])
{
	int retval;
	int tube[2];
	int ack_tube[2];
	pid_t myPID;

	// create pipe to be shared between parent and child processes
	retval = pipe(tube);
	if (-1 == retval) 
		perror("Failed to create pipe tube");

	retval = pipe(ack_tube);
	if (-1 == retval) 
		perror("Failed to create pipe ack_tube");

	// split process into a parent and a child
	myPID = fork();
	if (-1 == myPID)
		perror("Failed to fork child");

	if (0 == myPID) {
		// child process
		two(tube, ack_tube);
	}
	else {
		// parent process
		one(tube, ack_tube);
	}

	return 0;
}
