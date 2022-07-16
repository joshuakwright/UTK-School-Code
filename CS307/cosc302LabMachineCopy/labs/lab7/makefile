CC = g++
CFLAGS = -Wall -g
BIN = Mazemake Mazesolve Mazeimage

all: $(BIN);

Mazemake: Mazemake.cpp dset.h dset.o
	$(CC) $(CFLAGS) -o $@ Mazemake.cpp dset.o

.cpp.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f Mazemake Mazesolve Mazeimage dset.o
	rm -f *.txt
	rm -f *.ppm

