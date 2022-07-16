
//Joshua Wright CS307 Lab 4 Gregor
//This program will take in a message and a ppm image and encode the message into the 
//image and output a new image with the encoded message that can then be fed back
//into the program and extract the message back out
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;

#include "PPM.h"
#include "Rnumgen.h"

const char ETX = 0x3;

struct pixel {
	pixel(int n_x=0, int n_y=0) {
		x = n_x;
		y = n_y;
	}

	int x;
	int y;
};

//creates map of pixels to be altered when encoding the message based off of even indexes 
void set_pixel_list(PPM &img, vector<pixel> &pixel_list) {
	int rows = img.get_Nrows();
	int cols = img.get_Ncols();

	for (int i = 0; i <= rows-1; i++) {
		for (int j = 0; j <= cols-1; j++) {
			if (i % 2 == 0 && j % 2 == 0) {
				pixel_list.push_back(pixel(i,j));
			}
		}
	}
}

//encodes the message into the picture by changing one bit of either the R, G, or B color in
//each pixel in pixel_list
void encode(PPM &img, vector<pixel> pixel_list) {
	char message;

	char *rgb;
	int k = -1;
	int n = 0;
	while (cin.get(message)) {
		for (int i = 0; i < 7; i++) {
			int x = pixel_list[n].x;
			int y = pixel_list[n].y;

			k = (k + 1) % 3;
			rgb = (((char *)&img[x][y]) + k);
			*rgb &= 0xFE;
			*rgb |= ((message >> i) & 0x1);

			n++;
		}
	}

	//encoding of termination character
	message = ETX;
	for (int i = 0; i < 7; i++) {
		int x = pixel_list[n].x;
		int y = pixel_list[n].y;

		k = (k + 1) % 3;
		rgb = ((char *)&img[x][y]) + k;

		*rgb &= 0xFE;
		*rgb |= ((message >> i) & 0x1);

		n++;
	}
}

//decode function simply reverses the encoding process, extracting each message bit from
//the pixels and then outputting the message character by character as it is extracted
void decode(PPM &img, vector<pixel> pixel_list) {
	char message;
	int k = -1;
	char *rgb;
	int n = 0;

	while (message != ETX) {
		message = 0x0;
		for (int i = 0; i < 7; i++) {
			int x = pixel_list[n].x;
			int y = pixel_list[n].y;

			k = (k + 1) % 3;
			rgb = ((char *)&img[x][y]) + k;


			message |= (*rgb & 0x1) << i;

			n++;
		}	

		if (message != ETX) {
			cout.put(message);
		}
	}
}

//error checks inputs and calls appropraite functions to either encode or decode a message
//into or out of an image
int main(int argc, char *argv[]) {
	if (argc != 3) {
		cerr << "Error: -encode|-decode image.ppm\n";
		return 0;
	}
	string mode = argv[1];
	string fname = argv[2];
	
	if (mode != "-encode" && mode != "-decode") {
		cerr << "Error: -encode|-decode image.ppm\n";
		return 0;
	}

	PPM img;
	img.read(fname);
	
	vector<pixel> pixel_list;
	set_pixel_list(img, pixel_list);

	if (mode == "-encode") {
		encode(img, pixel_list);
		img.write(fname);
	}
	else if (mode == "-decode") {
		decode(img, pixel_list);
	}
}
