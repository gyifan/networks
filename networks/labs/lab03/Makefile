CC       = gcc #-DDEBUG
HDRS     = wrappers.h tcplib.h
SERVOBJS = reflectord.o
PINGOBJS = pingpong.o
WRPOBJS  = wrappers.o
TCPOBJS  = tcplib.o
OBJS     = $(SERVOBJS) $(PINGOBJS) $(WRPOBJS) $(TCPOBJS)
CFLAGS   = -Wall -g
EXECS    = reflectord pingpong

LFLAGS   = # add your loader flags when needed

all: $(EXECS)

# The following line will compile all *.c code to corresponding *.o file
%.o: %.c $(HDRS)
	$(CC) -c $(CFLAGS) $<

reflectord: $(SERVOBJS) $(WRPOBJS) $(TCPOBJS)
	$(CC) -o $@ $(SERVOBJS) $(LFLAGS) $(WRPOBJS) $(TCPOBJS)

pingpong: $(PINGOBJS) $(WRPOBJS) $(TCPOBJS)
	$(CC) -o $@ $(PINGOBJS) $(LFLAGS) $(WRPOBJS) $(TCPOBJS)

clean:
	/bin/rm -f $(OBJS) $(EXECS) core* *~ semantic.cache
