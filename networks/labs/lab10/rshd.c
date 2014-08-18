/*
 * rshd.c - main
 */
#include <stdio.h>
#include <unistd.h>
#include "tcplib.h"
#define RSHPORT         2527       /* a non-privilege port so all can use */
#define MAX             1024

int main(int argc,char *argv[])
{
    int s, t;
    int pid;
    extern void rshService(int);

    s = socketServer(RSHPORT);
    while (1)
    {
	if (s > 0)
	    t = acceptConn(s);
	else
	{
	    fprintf(stderr," socket error\n");
	    exit(1);
	}
	if (t > 0)
	{
	  pid = fork();
	  if (pid == 0) /* child process continue */
	    {
	      rshService(t);
	      //		  close(t);
	      shutdown(t, 2);
	      exit(0);
	    }
	}
	else
	    fprintf(stderr," connection error\n");
    } /* while(1) */
    close(s);
    return 0;
}
