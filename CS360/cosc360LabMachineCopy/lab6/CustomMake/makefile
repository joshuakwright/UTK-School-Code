CC = gcc
CFLAGS = -g -Iex/

testmain: src/testmain.c ex/mymalloc.h src/mymalloc.c
	$(CC) $(CFLAGS) -o $@ src/testmain.c src/mymalloc.c

clean:
	rm core $(EXECUTABLES) *.o