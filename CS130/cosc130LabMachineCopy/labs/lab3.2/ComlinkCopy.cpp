#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

typedef unsigned char uchar;

// Hamming encoding packages data and parity bits
//
// Input:  0  0  0  0  d3 d2 d1 d0
// Input:  0  0  0  0  0  p2 p1 p0 
// Output: 0  d3 d2 d1 p2 d0 p1 p0

uchar hamming_encode(uchar d, uchar p) { return 0x0; }

// Hamming decoding: extract parity bits
//
// Input:  0  d3 d2 d1 p2 d0 p1 p0
// Output: 0  0  0  0  0  p2 p1 p0

uchar hamming_getparity(uchar c) { 0x0; }

// Hamming decoding: extract data bits
//
// Input:  0  d3 d2 d1 p2 d0 p1 p0
// Output: 0  0  0  0  d3 d2 d1 d0

uchar hamming_getdata(uchar c) { 0x0; }

// Bad correction calculates which bit in the hamming
// encoded word needs to be corrected by a complement
// operation. The bit index is the sum of the indices 
// of the parity bits which are different from their 
// expected value (ps vs pb below). 
//
// Index:   8  7  6  5  4  3  2  1
// Hamming: 0  d3 d2 d1 p2 d0 p1 p0
// Data:    -  c3 c2 c1 -  c0 -  -
//
// Thus, if no parity bits come out different from their
// expected values, all data bits are good. If, on the
// other hand, say p0 and p2 come out different, then 
// d1 needs to be flipped (1+4=5). That in turn means 
// c1 since the bits have been extracted and repackaged.

uchar badbit_correction(uchar c, uchar pa, uchar pb) { return 0x0; }


int main(int argc, char *argv[]) {
  if (argc != 2 || strlen(argv[1]) == 0) {
    cerr << "usage: " << argv[0] << " magic_key\n";
    return 0;
  }

  string magic_key = argv[1];
  int NMK = magic_key.length();

  declare and fill parity[16] array

  declare data buffer 1
  declare data buffer 2

  if (decrypting) {
    skip 0x0 byte

    while (1) {
	  read data from cin

	  if (no bytes read and EOF is true)
	    break;

	  for (each byte pair read) {
		extract parity bits for byte one
		extra data bits for byte one

		correct any bad data bits for byte one

		extract parity bits for byte two
		extra data bits for byte two

		correct any bad data bits for byte two

		form encrypted character from bytes one, two

		xor-decrypt character 
		write decrypted characted to buffer
      }
  
	  write buffer to stdout
    }

  } else {
	write encryption marker to stdout

    while (1) {
	  read data from stdin

	  if (no bytes read and EOF is true)
	    break;
  
	  for (each byte read) {
		xor-encrypt character 

		extract high half-byte (0xf0)
		extract low half-byte  (0x0f)

		determine parity for high half-byte
		determine parity for high low-byte

		write hamming encoded high half-byte to buffer
		write hamming encoded low  half-byte to buffer
      }
  
      write buffer to stdout
    }
  }
}
