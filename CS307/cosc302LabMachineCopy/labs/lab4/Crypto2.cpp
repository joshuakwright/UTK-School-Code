
//Joshua Wright CS307 Lab 4 Gregor
//This program behaves the same as crypto1 exept it uses the inputed image 
//and an inputted seed value to randomly permute the location of each encoded pixel
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

void set_pixel_list(PPM &img, vector<pixel> &pixel_list, int seedvalue) {
	int rows = img.get_Nrows();
	int cols = img.get_Ncols();

	for (int i = 0; i <= rows-1; i++) {
		for (int j = 0; j <= cols-1; j++) {
			pixel_list.push_back(pixel(i,j));
		}
	}

	//this creates a histogram of color values throughout the image based on the 
	//frequecy of occurance of a color created by using the 4 most significant bits
	//of every pixel's R, G, and B color combined into one twelve bit number
	vector<int> histogram(4096, 0);

	int color = 0;
	for (int i = 0; i <= rows-1; i++) {
		for (int j = 0; j <= cols-1; j++) {
			color = (img[i][j].R >> 3);
			color = color << 3;
			color |= (img[i][j].G >> 3);
			color = color << 3;
			color |= (img[i][j].B >> 3);
			histogram[color]++;
			color = 0;
		}
	}

	//the histogram along with an inputted seed number is used to see a random number 
	//generator that creates numbers used to permute all the pixel locations
	rnumgen randnum(seedvalue, histogram);

	for (int i = (int)pixel_list.size() - 1; i > 0; --i) {
		int randnum1 = randnum.rand();
		int randnum2 = randnum.rand();
		int randnum24 = (randnum1 << 12) | randnum2;

		swap(pixel_list[i], pixel_list[randnum24 % (i+1)]);

	}
}

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

int main(int argc, char *argv[]) {
	if (argc != 3 && argc != 4) {
		cerr << "Error: -encode|-decode [-seed=value] image.ppm\n";
		return 0;
	}
	string mode = argv[1];
	string fname;
	int seed = 0;
	if (argc == 3) {
		fname = argv[2];
	}
	//inputted seed value is read in as a string so the seed is extracted
	//and turned into an int
	else if (argc == 4) {
		fname = argv[3]; 
		string ssead = argv[2];
		ssead.erase(ssead.begin(), ssead.begin() + 6);
		istringstream sin(ssead);
		sin >> seed;
		sin.clear();
	}
	if (mode != "-encode" && mode != "-decode") {
		cerr << "Error: -encode|-decode [-seed=value] image.ppm\n";
		return 0;
	}

	PPM img;
	img.read(fname);

	vector<pixel> pixel_list;
	set_pixel_list(img, pixel_list, seed);

	if (mode == "-encode") {
		encode(img, pixel_list);
		img.write(fname);
	}
	else if (mode == "-decode") {
		decode(img, pixel_list);
	}
}
