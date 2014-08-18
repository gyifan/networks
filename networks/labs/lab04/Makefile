CC       = gcc #-DDEBUG
HDRS     = wrappers.h tcplib.h
SERVOBJS = reflectord.o
PINGOBJS = pingpong.o
WRPOBJS  = wrappers.o
LISTOBJS = list.o
SENDOBJS = list-sender.o
RECVOBJS = list-receiver.o
TCPOBJS  = tcplib.o
OBJS     = $(SERVOBJS) $(PINGOBJS) $(WRPOBJS) $(SENDOBJS) $(RECVOBJS) $(TCPOBJS)
CFLAGS   = -Wall -g
EXECS    = reflectord pingpong list-sender list-receiver

LFLAGS   = # add your loader flags when needed

all: $(EXECS)

# The following line will compile all *.c code to corresponding *.o file
%.o: %.c $(HDRS)
	$(CC) -c $(CFLAGS) $<

reflectord: $(SERVOBJS) $(WRPOBJS) $(TCPOBJS)
	$(CC) -o $@ $(SERVOBJS) $(LFLAGS) $(WRPOBJS)

pingpong: $(PINGOBJS) $(WRPOBJS) $(TCPOBJS)
	$(CC) -o $@ $(PINGOBJS) $(LFLAGS) $(WRPOBJS)

list-sender: $(SENDOBJS) $(WRPOBJS) $(TCPOBJS) $(LISTOBJS)
	$(CC) -o $@ $(SENDOBJS) $(LFLAGS) $(WRPOBJS) $(TCPOBJS) $(LISTOBJS)

list-receiver: $(RECVOBJS) $(WRPOBJS) $(TCPOBJS) $(LISTOBJS)
	$(CC) -o $@ $(RECVOBJS) $(LFLAGS) $(WRPOBJS) $(TCPOBJS) $(LISTOBJS)

clean:
	/bin/rm -f $(OBJS) $(EXECS) core* *~ semantic.cache
