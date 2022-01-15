CC=gcc
CFLAGS=-O3 -fPIC -g
MALLOC_VERSION=FF
WDIR=..
TARGET = control_manager_test

all: $(TARGET)

control_manager_test: control_manager_test.c
	$(CC) $(CFLAGS) -I$(WDIR) -L$(WDIR) -D$(MALLOC_VERSION) -Wl,-rpath=$(WDIR) -o $@ $< -lmymalloc -lrt

clean:
	rm -f *~ *.o $(TARGET)

clobber:
	rm -f *~ *.o
