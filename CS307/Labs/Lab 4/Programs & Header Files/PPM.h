#ifndef __PPM_H__
#define __PPM_H__

#include <fstream>
#include <iostream>
#include <sstream>

struct RGB { 
  RGB() { R = '0'; G = '0'; B = '0';}

  unsigned char R, G, B;
};

class PPM { 
  public:
    PPM() { Nrows = 0; Ncols = 0; maxval = 0;}
    ~PPM() { 
		delete[] img[0];
		delete[] img;
	}

	void read(string fname);
	void write(string fname);

	RGB *operator[](int row) { return img[row]; }

	int get_Nrows() { return Nrows; }
	int get_Ncols() { return Ncols; }

  private:
	int Nrows;
	int Ncols;
	int maxval;
	RGB **img;
};

#endif
