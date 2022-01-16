CC=gcc
CFLAGS= -fPIC -g
DEPS=my_malloc.h

.PNONY=lib debug
all: lib

lib: my_malloc.o
	$(CC) $(CFLAGS) -shared -o libmymalloc.so my_malloc.o

%.o: %.c my_malloc.h
	$(CC) $(CFLAGS) -c -o $@ $<
debug: my_malloc.c my_malloc.h
	$(CC) $(CFLAGS) -DNDEBUG -c $<
	$(CC) $(CFLAGS) -shared -o libmymalloc.so my_malloc.o

clean:
	rm -f *~ *.o *.so

clobber:
	rm -f *~ *.o
