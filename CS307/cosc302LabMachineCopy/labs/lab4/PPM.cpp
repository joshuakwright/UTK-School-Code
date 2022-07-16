
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

#include "PPM.h"

//read function opens file, reads in header info, checks for errors, creates a 2d
//array of pionter and RGB objects, and reads in the file using binary io
void PPM::read(string fname) { 
	ifstream fin;
	fin.open(fname.c_str());
	if (fin.fail()) {
		cerr << "Failed to open file " << fname << endl;
		return;
	}

	string P6;
	fin >> P6 >> Ncols >> Nrows >> maxval;
	char newline;
	fin.get(newline); 

	if (P6 != "P6" || maxval > 255 || maxval < 0) {
		cerr << "Header Error\n";
		return;
	}
	img = new RGB*[Nrows];
	img[0] = new RGB[Nrows*Ncols];
	
	for (int i = 1; i <= Nrows-1; i++) {
		img[i] = img[i-1] + Ncols;
	}
	
	fin.read((char *)img[0], 3*Ncols*Nrows);
	if (fin.gcount() != 3*Nrows*Ncols) {
		cerr << "File Read Error\n";
		return;
	}

	fin.close();
}

//write function appends _wmsg.ppm to the file name, opens the new output file, and writes 
//header info and the altered pixels with the encoded message to the file
void PPM::write(string fname) { 
	ofstream fout;
	string ofile = fname;
	ofile.erase(ofile.end() - 4, ofile.end());
	ostringstream sout;
	sout << ofile << "_wmsg.ppm";
	ofile = sout.str();
	sout.str("");
	fout.open(ofile.c_str());

	if (!fout.is_open()) {
		cerr << "File Write Error\n";
	}

	fout << "P6\n" << Ncols << " " << Nrows << endl << maxval << "\n";

	fout.write((char *)img[0], 3*Ncols*Nrows);

	fout << endl;

	fout.close();
}
