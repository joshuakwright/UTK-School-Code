#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

typedef unsigned char uchar;

uchar hamming_encode(uchar d, uchar p) { 
	uchar encoded;
	uchar d3 = (d >> 3);
	uchar d2 = ((d >> 2) & 0x1);
	uchar d1 = ((d >> 1) & 0x1);
	uchar d0 = (d & 0x1);
	uchar p2 = ((p >> 2) & 0x1);
	uchar p1 = ((p >> 1) & 0x1);
	uchar p0 = (p & 0x1);
	encoded = ((d3 << 6) | (d2 << 5) | (d1 << 4) | (p2 << 3) | (d0 << 2) | (p1 << 1) | p0);

	return encoded; 
}

uchar hamming_getparity(uchar c) { 
	uchar p;
	uchar p2 = ((c >> 3) & 0x01);
	uchar p1 = ((c >> 1) & 0x01);
	uchar p0 = (c & 0x01);
	p = ((p2 << 2) | (p1 << 1) | p0);

	return p;
}

uchar hamming_getdata(uchar c) { 
	uchar d;
	uchar d3 = ((c >> 6) & 0x01);
	uchar d2 = ((c >> 5) & 0x01);
	uchar d1 = ((c >> 4) & 0x01);
	uchar d0 = ((c >> 2) & 0x01);
	d = ((d3 << 3) | (d2 << 2) | (d1 << 1) | d0);

	return d;
}

uchar badbit_correction(uchar c, uchar pa, uchar pb) { 
	uchar correct = c;
	uchar test = (pa ^ pb);
	uchar p2 = (test & 0x04);
	uchar p1 = (test & 0x02);
	uchar p0 = (test & 0x01);
	int two = 0;
	int one = 0;
	int zero = 0;
	int flip = 0;

	if (p2 == 0x04) {
		two = 4;
	}
	if (p1 == 0x02) {
		one = 2;
	}
	if (p0 == 0x01) {
		zero = 1;
	}

	flip = (two + one + zero);

	if (flip == 7) {
		correct ^= 0x08;
	}
	else if (flip == 6) {
		correct ^=  0x04;
	}
	else if (flip == 5) {
		correct ^= 0x02;
	}
	else if (flip == 3) {
		correct ^= 0x01;
	}

	return correct;
}


int main(int argc, char *argv[]) {
  if (argc != 2 || strlen(argv[1]) == 0) {
    cerr << "usage: " << argv[0] << " magic_key\n";
    return 0;
  }

  string magic_key = argv[1];
  int NMK = magic_key.length();

  char parity[16];
    for (int k = 0; k < 16; k++) {
		char l = (k & 0x0f);
		char p2 = ((((l & 0x08) >> 3) ^ ((l & 0x04) >> 2) ^ ((l & 0x02) >> 1)) << 2);
		char p1 = ((((l & 0x08) >> 3) ^ ((l & 0x04) >> 2) ^ (l & 0x01)) << 1);
		char p0 = (((l & 0x08) >> 3) ^ ((l & 0x02) >> 1) ^ (l & 0x01));
		parity[k] = (p2 | p1 | p0);
	}

/*	  Parity Table
 * K	D3D2D1D0	0P2P1P0
 * 0	0 0 0 0		0 0 0 0
 * 1	0 0 0 1		0 0 1 1
 * 2	0 0 1 0		0 1 0 1
 * 3	0 0 1 1		0 1 1 0
 * 4	0 1 0 0		0 1 1 0
 * 5	0 1 0 1		0 1 0 1
 * 6	0 1 1 0		0 0 1 1
 * 7	0 1 1 1		0 0 0 0
 * 8	1 0 0 0		0 1 1 1
 * 9	1 0 0 1		0 1 0 0
 * 10	1 0 1 0		0 0 1 0
 * 11	1 0 1 1		0 0 0 1
 * 12	1 1 0 0		0 0 0 1
 * 13	1 1 0 1		0 0 1 0
 * 14	1 1 1 0		0 1 0 0
 * 15	1 1 1 1		0 1 1 1
 */

  if (cin.peek() == (0x00)) {
	char buffere[256];
	char bufferu[128];
	char bufferp1[256];
	char bufferd[256];
	char bufferp2[256];
	cin.get();
    while (1) {
		cin.read(buffere, 256);

	  if ((cin.eof()) && (cin.gcount() == 0)) {
	    break;
	  }

	  for (int i = 0; i < (cin.gcount() / 2); i++) {
		bufferp1[i*2] = hamming_getparity(buffere[i*2]);
		bufferd[i*2] = hamming_getdata(buffere[i*2]);

		int hi = bufferd[i*2];		
		bufferp2[i*2] = parity[hi];

		buffere[i*2] = badbit_correction(bufferd[i*2], bufferp2[i*2], bufferp1[i*2]);
	
		bufferp1[i*2+1] = hamming_getparity(buffere[i*2+1]);
		bufferd[i*2+1] = hamming_getdata(buffere[i*2+1]);
		
		int lo = bufferd[i*2+1];
		bufferp2[i*2+1] = parity[lo];

		buffere[i*2+1] = badbit_correction(bufferd[i*2+1], bufferp2[i*2+1], bufferp1[i*2+1]);

		bufferu[i] = (buffere[i*2] << 4) | buffere[(i*2)+1];

		bufferu[i] = bufferu[i] ^ magic_key[i%NMK];
	  }
  
	  cout.write(bufferu, cin.gcount() / 2);
    }

  } 
  
  else {
	char buffere[256];		
	char bufferu[128];
	char bufferp[256];
	cout.put(0x00);
    while (1) {
	  cin.read(bufferu, 128);

	  if ((cin.eof()) && (cin.gcount()) == 0)
	    break;
  
	  for (int j = 0; j < cin.gcount(); j++) {
		
		bufferu[j] = bufferu[j] ^ magic_key[j%NMK];

		buffere[j*2] = (bufferu[j] & 0xf0) >> 4;
		buffere[j*2+1] = bufferu[j] & 0x0f;

		int hi = buffere[j*2];
		int lo = buffere[j*2+1];
		bufferp[j*2] = parity[hi];
		bufferp[j*2+1] = parity[lo];

		buffere[j*2] = hamming_encode(buffere[j*2], bufferp[j*2]);
		buffere[j*2+1] = hamming_encode(buffere[j*2+1], bufferp[j*2+1]);
      }
  
      cout.write(buffere, cin.gcount() * 2);
    }
  }
}
