CC = g++
CFLAGS = -Wall -g

all: Crypto3

Crypto3: Crypto3.cpp PPM.h PPM.o Rnumgen.h Rnumgen.o
	$(CC) $(CFLAGS) -o $@ Crypto3.cpp PPM.o Rnumgen.o 

.cpp.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f Crypto3 PPM.o Rnumgen.o
	rm -f *_wmsg.ppm
