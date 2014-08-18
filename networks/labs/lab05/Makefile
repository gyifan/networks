CC   = gcc
H    = tcplib.h wrappers.h
LIB  = #-lnsl -lsocket
CFLG = -g
ALL  = superServer timeClient-udp echoClient getwho

all: $(ALL)

%.o: %.c $(H)
	$(CC) -c $(CFLG) $<

superServer: superServer.o tcplib.o wrappers.o
	$(CC) -o superServer superServer.o tcplib.o wrappers.o $(LIB)

timeClient-udp: timeClient-udp.o 
	$(CC) -o timeClient-udp timeClient-udp.o $(LIB)

echoClient: echoClient.o tcplib.o
	$(CC) -o echoClient echoClient.o tcplib.o $(LIB)

getwho: getwho.o tcplib.o wrappers.o
	$(CC) -o getwho getwho.o tcplib.o wrappers.o $(LIB)

clean:
	/bin/rm -rf *.o core *~ $(ALL)
