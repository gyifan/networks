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
 * Revision history:
 * 1. Xiannong Meng: 01-04-2013
 *    -- Remove the 'typedef' for now, introduce it later.
 *    -- Terminate the string generated by 'strncpy.'
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

struct buffer {
  int a;
  double b;
  short c;
  char line[80];
};


int main(int argc, char* argv[])  {

  struct buffer myBuffer;
  int myFile;
  int retval;
  char* myString = "Ahoy there!";
  int len = strlen(myString);

  /* initialize buffer */
  myBuffer.a = 123;
  myBuffer.b = 1.1314926;
  myBuffer.c = 9;

  strncpy((char*) &(myBuffer.line), myString, len + 1);
  myBuffer.line[len + 1] = 0;   // terminate the string

  myFile = open("myfile", O_CREAT|O_WRONLY, S_IREAD|S_IWRITE);

  if (-1 == myFile)
    perror("File operation failed");

  retval = write(myFile, &myBuffer, sizeof(myBuffer));
  retval = write(myFile, &myBuffer, sizeof(myBuffer));
  retval = write(myFile, &myBuffer, sizeof(myBuffer));

  retval = close(myFile);

  if (retval == -1)
    perror("File operation failed");

  return 0;
}
