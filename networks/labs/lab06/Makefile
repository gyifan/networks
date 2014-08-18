CC       = gcc #-DDEBUG
HDRS     = wrappers.h tcplib.h
SERVOBJS = sink.o
CLITOBJS = source.o
GATEOBJS = gateway.o
WRPOBJS  = wrappers.o
TCPOBJS  = tcplib.o
OBJS     = $(SERVOBJS) $(CLITOBJS) $(WRPOBJS)
CFLAGS   = -Wall -g
EXECS    = source sink gateway

LFLAGS   = # add your loader flags when needed

all: $(EXECS)

# The following line will compile all *.c code to corresponding *.o file
%.o: %.c $(HDRS)
	$(CC) -c $(CFLAGS) $<

sink: $(SERVOBJS) $(WRPOBJS)
	$(CC) -o $@ $(SERVOBJS) $(LFLAGS) $(WRPOBJS)

source: $(CLITOBJS) $(WRPOBJS)
	$(CC) -o $@ $(CLITOBJS) $(LFLAGS) $(WRPOBJS)

gateway: $(GATEOBJS) $(WRPOBJS)
	$(CC) -o $@ $(GATEOBJS) $(LFLAGS) $(WRPOBJS)

clean:
	/bin/rm -f $(OBJS) $(EXECS) core* *~ semantic.cache
