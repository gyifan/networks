CC     = gcc
CFLAGS = -g
LFLAGS =

HDRS   = sag-api.h crcmodel.h wrappers.h
SAGOBJ = sag-api.o
LSTOBJ = list.o
CRCOBJS = crc-api.o crcmodel.o
SERVOBJS = server.o
CLITOBJS = client.o
WRPOBJ  = wrappers.o
OBJS     = $(SERVOBJS) $(CLITOBJS) $(WRPOBJ) $(CRCOBJS) $(SAGOBJ) $(LSTOBJ)


EXECS    = client server 

all: $(EXECS)

%.o: %.c $(HDRS)
	$(CC) -c $(CFLAGS) $<

server: $(SERVOBJS) $(WRPOBJ) $(CRCOBJS) $(SAGOBJ) $(LSTOBJ)
	$(CC) -o $@ $(SERVOBJS) $(LFLAGS) $(WRPOBJ) $(CRCOBJS) $(SAGOBJ) $(LSTOBJ)

client: $(CLITOBJS) $(WRPOBJ) $(SAGOBJ) $(CRCOBJS) $(LSTOBJ)
	$(CC) -o $@ $(CLITOBJS) $(LFLAGS) $(WRPOBJ) $(SAGOBJ) $(CRCOBJS) $(LSTOBJ)

clean:
	/bin/rm -f $(OBJS) $(EXECS) core* *~ semantic.cache

