CC=gcc
CFLAGS=-fPIC -g
WDIR=..
SRCS = $(wildcard *.c)
TARGETS= $(patsubst %.c, %, $(SRCS))

all: $(TARGETS)

%:%.c 
	$(CC) $(CFLAGS) -I$(WDIR) -L$(WDIR) -Wl,-rpath=$(WDIR) -o $@ $< -lmymalloc 

clean:
	rm -f *~ *.o $(TARGETS)

clobber:
	rm -f *~ *.o
