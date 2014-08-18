CC       = gcc
HDRS     = wrappers.h lab2.h# add your header files here when needed
SERVOBJS = fileserver.o
GETSOBJS = getfile.o
GETSTATOBJS = getstats.o
WRPOBJS  = wrappers.o
OBJS     = $(SERVOBJS) $(GETSOBJS) $(WRPOBJS) $(GETSTATOBJS)
CFLAGS   = -Wall -g
EXECS    = fileserver getfile getstats

LFLAGS   = # add your loader flags when needed

all: $(EXECS)

# The following line will compile all *.c code to corresponding *.o file
%.o: %.c $(HDRS)
	$(CC) -c $(CFLAGS) $<

fileserver: $(SERVOBJS) $(WRPOBJS)
	$(CC) -o $@ $(SERVOBJS) $(LFLAGS) $(WRPOBJS)

getfile: $(GETSOBJS) $(WRPOBJS)
	$(CC) -o $@ $(GETSOBJS) $(LFLAGS) $(WRPOBJS)

getstats: $(GETSTATOBJS) $(WRPOBJS)
	$(CC) -o $@ $(GETSTATOBJS) $(LFLAGS) $(WRPOBJS)

clean:
	/bin/rm -f $(OBJS) $(EXECS) core* *~ semantic.cache
