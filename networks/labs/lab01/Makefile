CC = gcc
OBJS = main.o foo.o 
CFLAGS = -Wall -g #-DDEBUG
EXECS =	fork-test file-test read-test two-way

all: $(EXECS)

fork-test: fork-test.c
	$(CC) -o fork-test $(CFLAGS) fork-test.c 

file-test: file-test.c
	$(CC) -o file-test $(CFLAGS) file-test.c 

read-test: read-test.c
	$(CC) -o read-test $(CFLAGS) read-test.c 

two-way: two-way.c
	$(CC) -o two-way $(CFLAGS) two-way.c

clean:
	/bin/rm -f $(OBJS) $(EXECS) core* *~
