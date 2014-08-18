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

#define FOREVER 1
#define INPUT   0
#define OUTPUT  1

/**************************************************************

 Source: a process that pushes a null-terminated string of 
   characters into a pipe.

***************************************************************/

void source(int* fd)
{
  int retval;
  char  one[80];
  char  two[80];
  int   count = 0;
  int   length;
  char* buffer;

  strcpy(one, "baseball is boring");
  strcpy(two, "soccer rules");
  buffer = one;
  length = strlen(one);

  printf("Source running...\n");
  
  // close read end of the pipe
  retval = close(fd[INPUT]);
  if (-1 == retval)
    perror("Error closing input pipe");

  while(FOREVER) {
    retval = write(fd[OUTPUT], &(buffer[count]), 1);
    if (-1 == retval)
      perror("Error writing to pipe");

    count++;

    if (count > length) {
      count = 0;
      if (buffer == one)  
	buffer = two;
      else
	buffer = one;
      
      length = strlen(buffer);
    }

    /* SOURCE DELAY */    
    sleep(1);
  } 
}


/**************************************************************

   Sink: a process that reads characters from a pipe one at a
   time and displays them on the screen.

***************************************************************/

void sink(int* fd)
{ 
  char line[80];
  int count;
  int retval;

  printf("Sink running...\n");

  count = 0;

  // close write end of the pipe
  retval = close(fd[OUTPUT]);
  if (-1 == retval)
    perror("Error closing output pipe");

  while(FOREVER) {
    // read one byte from pipe and store it in buffer
    retval = read(fd[INPUT], &(line[count]), 1);
    if (-1 == retval)
      perror("Error reading from pipe");
    
    if (0 == line[count]) {
      printf("line = %s\n", line);
      count = 0;

      /* PRINT TIME WHEN FULL MESSAGE RECEIVED */
    }
    else 
      count++; 
  }
}


/**************************************************************

 main: creates a pipe and two processes (one sink and one source).

***************************************************************/

int main(int argc, char* argv[])
{
  int retval;
  int tube[2];
  pid_t myPID;

  // create pipe to be shared between parent and child processes
  retval = pipe(tube);
  if (-1 == retval) 
    perror("Failed to create pipe");

  // split process into a parent and a child
  myPID = fork();
  if (-1 == myPID)
    perror("Failed to fork child");

  if (0 == myPID) {
    // child process
    sink(tube);
  }
  else {
    // parent process
    source(tube);
  }

  return 0;
}
