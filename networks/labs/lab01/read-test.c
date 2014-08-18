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
 *
 * Revision history:
 * 1. Xiannong Meng: 01-04-2013
 *    -- Remove the 'typedef' for now, introduce it later.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct buffer {
  int a;
  double b;
  short c;
  char line[80];
};

int main(int argc, char* argv[])    {

  if(argc != 2){
	printf("Usage: ./read-test [FILE]\n");
	exit(-1);
  }

  struct buffer myBuffer;
  int myFile;
  int retval;

  myFile = open(argv[1], O_RDWR);

  if(-1 == myFile){
	perror("Opening file failed\n");
	exit(-1);
  }
  
  retval = read(myFile, &myBuffer, sizeof(myBuffer));

  if(sizeof(myBuffer) != retval){
	printf("inconsistant data type\n");
	exit(-1);
  }

  printf("\n bytes read: %d\n", retval);

  printf("\nFile name: %s\n", argv[1]);
  printf("Buffer contents:\n");
  printf("   int     a = %d\n", myBuffer.a);
  printf("   double  b = %lf\n", myBuffer.b);
  printf("   short   c = %d\n", myBuffer.c);
  printf("   char line = %s\n", myBuffer.line);
  printf("---------------------\n");
  printf("   size of struct = %ld\n", sizeof(myBuffer));
  printf("   bytes read     = %d\n", retval);
  
  retval = close(myFile);
  
  return 0;
}
