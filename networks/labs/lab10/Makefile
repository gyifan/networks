CC   = gcc
NLIB = -lcrypt #-lnsl -lsocket -lcurses
CLIB = -lcrypt #-lcurses
EXE  = rshd rshClient mypasswd
CFLAGS    = -Wall -g

all: $(EXE) 

%.o: %.c
	$(CC) -c $(CFLAGS) $<

rshd: rsh.o rshd.o tcplib.o shadow.o pwd.o login.o
	$(CC) -o rshd rsh.o rshd.o shadow.o pwd.o tcplib.o login.o $(NLIB)

rshClient: rshClient.o tcplib.o
	$(CC) -o rshClient rshClient.o tcplib.o $(NLIB)

mypasswd: mypasswd.o shadow.o pwd.o
	$(CC) -o mypasswd mypasswd.o shadow.o pwd.o $(CLIB)

clean:
	/bin/rm -rf *~ *.o core $(EXE)
