CC=gcc
CFLAGS=-pthread
OBJS=structs.o io.o conv.o time.o

all: program1 program2 program3

program1: program1.o $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)
program2: program2.o $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)
program3: program3.o $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c
	$(CC) -c -o $@ $<

clean:
	rm -f program1 program2 program3 *.o
