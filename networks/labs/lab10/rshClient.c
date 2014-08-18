/* rshClient - remote shell client                                */
/*                                                                */
/* compile with -lcurses                                          */
#include <stdio.h>
#include <signal.h>
#include <sgtty.h>
#include <sys/types.h>
#include <unistd.h>
#include "tcplib.h"

#include <termios.h>

#define RSHPORT 2527
#define BUFLEN 30
int main(int argc, char *argv[])
{
  void passAll(int, char*);

  int sock;
  char *host;
  //  struct sgttyb args;
  char prompt[BUFSIZ];

  if (argc != 2)
  {
    fprintf(stderr,"usage : %s server-name\n", argv[0]);
    exit(1);
  }

  host = argv[1];
  sock = socketClient(host, RSHPORT);
  if (sock <= 0)
  {
    fprintf(stderr,"connection failed\n");
    exit(2);
  }


  /* build the prompt */
  strcpy(prompt, host);
  strcat(prompt, "% ");

  passAll(sock, prompt);

  /* close the socket */
  close(sock);

  return 0;
}

void passAll(int sock, char * prompt)
{
  int maxfdp1, nfound, nread;
  char buff[BUFSIZ+1];
  fd_set readmask;
  int n = strlen(prompt);
  
  int password = 0;
  struct termios newtty, savedtty;
  /* save the original terminal setting */
  tcgetattr(0, &savedtty);
  newtty = savedtty;

  FD_ZERO(&readmask);
  
  for (; ;)
  {

    if(strncmp(buff, "Password:",9) == 0 || strncmp(buff, "Username:", 9) == 0); 
    else write(1, prompt, n); /* print the prompt */

    FD_SET(0, &readmask);
    FD_SET(sock, &readmask);
    maxfdp1 = sock + 1;

    nfound = select(maxfdp1, &readmask, (fd_set *)0, (fd_set*)0,
		    (struct timeval *)0);

    if (nfound < 0)
    {
      exit(3);
    }
    
    if (FD_ISSET(0, &readmask))
    {
      /* 
       * check to see if stdin has data, if so read and pass it to the 
       * network server rshd.
       */
      nread = read(0, buff, BUFSIZ);
      if (nread < 0)
	exit(4);
      else if (nread == 0)
	break;
      
      if (write(sock, buff, nread) != nread)
	exit(5);
      
      /* Restore to original shell setting */ 
      if (password){
        tcsetattr(0, TCSANOW, &savedtty);
        write(1, "\n", 1);
        password = 0;
      }
      /* if user enters exit, close the session */
      if (strncmp(buff, "exit", 4) == 0)
        break;
    }

      /* 
       * check to see if socket has data, if so read and pass it to the 
       * local screen.
       */
    if (FD_ISSET(sock, &readmask))
    {
      nread = read(sock, buff, BUFSIZ);
      if (nread <= 0)
	break;
      
      if (strncmp(buff, "Password:", 9) == 0){
        newtty.c_lflag &= ~ECHO; // turn off echoing
        tcsetattr(0, TCSANOW, &newtty);
        password = 1;
      }
      
      if (write(1, buff, nread) != nread)
	exit(6);
    }
  }
}
