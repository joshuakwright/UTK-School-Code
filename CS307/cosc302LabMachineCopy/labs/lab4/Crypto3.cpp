
//Joshua Wright CS307 Lab 4 Gregor
//This does the same thing as crypto 2 but it allows for the input of a key which will be 
//randomly permuted and then used to randomly permute the encoded message as well as allowing
//for the input of another image that can be used to randomly permute the list of pixel indices
//as added layers of protection. It is reasonable impossible to unencode the message in an 
//image without all the propper inputs once it has been encoded by this program.
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

//a secondary image is inputted into this function to randomly permute the pixels
//intdependent of the image having the message encoded into it
void set_pixel_list(PPM &seedpic, PPM &img, vector<pixel> &pixel_list, int seedvalue) {
	int rows = img.get_Nrows();
	int cols = img.get_Ncols();

	for (int i = 0; i <= rows-1; i++) {
		for (int j = 0; j <= cols-1; j++) {
			pixel_list.push_back(pixel(i,j));
		}
	}

	vector<int> histogram(4096, 0);

	int color = 0;
	rows = seedpic.get_Nrows();
	cols = seedpic.get_Ncols();
	for (int i = 0; i <= rows-1; i++) {
		for (int j = 0; j <= cols-1; j++) {
			color = (seedpic[i][j].R >> 3);
			color = color << 3;
			color |= (seedpic[i][j].G >> 3);
			color = color << 3;
			color |= (seedpic[i][j].B >> 3);
			histogram[color]++;
			color = 0;
		}
	}

	rnumgen randnum(seedvalue, histogram);

	for (int i = (int)pixel_list.size() - 1; i > 0; --i) {
		int randnum1 = randnum.rand();
		int randnum2 = randnum.rand();
		int randnum24 = (randnum1 << 12) | randnum2;

		swap(pixel_list[i], pixel_list[randnum24 % (i+1)]);

	}
}

void encode(PPM &img, vector<pixel> pixel_list, string key) {
	char message;

	char *rgb;
	int k = -1;
	int n = 0;
	int j = 0;
	while (cin.get(message)) {
		//the message charaters are xor with the key characters in a rotating fashion
		message = message ^ key.at(j);
		for (int i = 0; i < 7; i++) {
			int x = pixel_list[n].x;
			int y = pixel_list[n].y;

			k = (k + 1) % 3;
			rgb = (((char *)&img[x][y]) + k);
			*rgb &= 0xFE;
			*rgb |= ((message >> i) & 0x1);

			n++;
		}

		j++;
		if (j >= (int)key.size()) {
			j = 0;
		}
	}

	message = ETX;
	message = message ^ key.at(j);
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

void decode(PPM &img, vector<pixel> pixel_list, string key) {
	char message;
	int k = -1;
	char *rgb;
	int n = 0;
	int j = 0;

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

		//the message characters are xor a second time with the key to 
		//unpermute them
		message = message ^ key.at(j);
		j++;
		if (j >= (int)key.size()) {
			j = 0;
		}

		if (message != ETX) {
			cout.put(message);
		}
	}
}

//this function permutes the characters in the key in the same way the 
//pixel list is permuted for an added layer of protection
void set_encryption_key(int seedvalue, string key) {
	vector<int> histogram(16, 0);
	for (int i = 0; i < (int)key.size(); i++) {
		int keynum = (int)(key.at(i) & 0xF);
		histogram[keynum]++;
	}

	rnumgen randnum(seedvalue, histogram);

	for (int i = key.size() - 1; i > 0; i--) {
		swap(key.at(i), key.at(randnum.rand() % (i+1)));
	}
}

int main(int argc, char *argv[]) {
	if (argc != 4 && argc != 5 && argc != 6) {
		cerr << "Error: -encode|-decode [-seed=value] [-key=value] image.ppm image.ppm\n";
		return 0;
	}
	string mode = argv[1];
	string fname;
	string sfname;
	string key = "GoVols!";
	int seed = 0;

	if (argc == 4) {
		fname = argv[3]; 
		sfname = argv[2];
	}

	else if (argc == 5) {
		string temp = argv[2];
		temp.erase(temp.begin() + 6);
		if (temp == "-key") {
			key = argv[2];
			key.erase(key.begin(), key.begin() + 5);
		}
		else {
			string ssead = argv[2];
			ssead.erase(ssead.begin(), ssead.begin() + 6);
			istringstream sin(ssead);
			sin >> seed;
			sin.clear();
		}
		sfname = argv[3];
		fname = argv[4];
	}

	else if (argc == 6) {
		sfname = argv[4];
		fname = argv[5];
		string ssead = argv[2];
		ssead.erase(ssead.begin(), ssead.begin() + 6);
		istringstream sin(ssead);
		sin >> seed;
		sin.clear();
		key = argv[3];
		key.erase(key.begin(), key.begin() + 5);
	}

	if (mode != "-encode" && mode != "-decode") {
		cerr << "Error: -encode|-decode [-seed=value] [-key=value] image.ppm image.ppm\n";
		return 0;
	}

	PPM img;
	img.read(fname);
	PPM seedpic;
	seedpic.read(sfname);

	vector<pixel> pixel_list;
	set_pixel_list(seedpic, img, pixel_list, seed);

	set_encryption_key(seed, key);

	if (mode == "-encode") {
		encode(img, pixel_list, key);
		img.write(fname);
	}
	else if (mode == "-decode") {
		decode(img, pixel_list, key);
	}
}
