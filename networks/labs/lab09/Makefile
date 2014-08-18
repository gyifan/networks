CC     = gcc
CFLAGS = -g
LFLAGS =

HDRS   = crcmodel.h wrappers.h
CRCOBJS = crc-api.o crcmodel.o
SERVOBJS = sink.o
CLITOBJS = source.o
GATEOBJS = gateway.o
WRPOBJS  = wrappers.o
OBJS     = $(SERVOBJS) $(CLITOBJS) $(WRPOBJS) $(CRCOBJS) $(GATEOBJS)


EXECS    = source sink gateway 

all: $(EXECS)

%.o: %.c $(HDRS)
	$(CC) -c $(CFLAGS) $<

sink: $(SERVOBJS) $(WRPOBJS) $(CRCOBJS)
	$(CC) -o $@ $(SERVOBJS) $(LFLAGS) $(WRPOBJS) $(CRCOBJS)

source: $(CLITOBJS) $(WRPOBJS) $(CRCOBJS)
	$(CC) -o $@ $(CLITOBJS) $(LFLAGS) $(WRPOBJS) $(CRCOBJS)

gateway: $(GATEOBJS) $(WRPOBJS)
	$(CC) -o $@ $(GATEOBJS) $(LFLAGS) $(WRPOBJS)

clean:
	/bin/rm -f $(OBJS) $(EXECS) core* *~ semantic.cache

