//main.cpp
//This lab reads in a list rgb values for a picture and then performs operations on them, like grey or invertion, and then outputs the altered rgb pixels to a new file.
//Joshua Wright
//3-Dec-2019
//Professor: Dr.Marz
//Students worked with:
//Dan Ervine
#include <iomanip>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

using namespace std;

//pixel structure
struct Pixel {
	int red;
	int green;
	int blue;
};

//picture class
class Picture {
	//private class members
	vector<vector<Pixel>> imageMap;
	int height;
	int width;
	int maxInt;
	
	//public class members
	public:
	Picture();
	void copy(vector<vector<Pixel>> &I) const;
	bool readFile(const string &R);
	int writeFile(const string &W);
	void yflip();
	void xflip();
	void neg(int N);
	void greys();
};

int main() {
	


}

//Picture constructer initializes Picture class private variables to 0
Picture::Picture() {
	height = 0;
	width = 0;
	maxInt = 0;
}

//picture class copy function copies the private Picture variable imageMap into another vector of vectors that is passed by refrence
void Picture::copy(vector<vector<Pixel>> &I) const {
	
	I.clear();
	
	int i;
	int j;
	I.resize(height);
	for (i = 0; i < height; i++) {
		I.at(i).resize(width);
		for (j = 0; j < width; j++) {
			I.at(i).at(j) = imageMap.at(i).at(j);
		}
	}
}

//this Picture class function reads in a file of rgb intigers and places them in the imageMap vector according to width and height requirements and then outputs true or false based on if there is an error
bool Picture::readFile(const string &R) {

	//variable declartion and file opened
	ifstream fin;
	fin.open(R);
	string line;
	istringstream sin;
	int i;
	int j;
	Pixel temp;
	bool a = true;
	
	//error checks if file doesnt open
	if (fin.fail()) {
		a = false;	
	}
	//error checks to make sure file header is correct
	getline(fin, line);
	if (line != "P3") {
		cout << "Error, not P3";
		a = false;
		return a;
	}
	//takes in the width and height from the file
	getline(fin, line);
	sin.str(line);
	sin >> width >> height;
	sin.clear();
	//takes in the maximum intensity for the file
	getline(fin, line);
	sin.str(line);
	sin >> maxInt;
	sin.clear();
	//sizes imageMap to correct height
	imageMap.resize(height);
	int pixelNum = 0;
	
	//for loops that put all the pixels int the imageMap vector and error checks the inensity
	for (i = 0; i < height; i++) {
		//sizes imageMap to correct width
		imageMap.at(i).resize(width);
		for (j = 0; j < width; j++) {
			getline(fin, line);
			//checks for commented lines and omits them
			if (line.at(0) == '#') {
				getline(fin, line);
			}
			//takes in rgb values for each pixel
			sin.str(line);
			sin >> temp.red >> temp.green >> temp.blue;
			sin.clear();
			//error checks for pixels over the max intensity
			if (temp.red > maxInt) {
				temp.red = maxInt;
				cout << "Warning, pixel greater than max intensity";
			}
			else if (temp.green > maxInt) {
				temp.green = maxInt;
			    cout << "Warning, pixel greater than max intensity";
			}	
			else if (temp.blue > maxInt) {
				temp.blue = maxInt;
				cout << "Warning, pixel greater than max intensity";
			}
			//adds each pixel to the imageMap vector and iterates pixelNum for error checking
			imageMap.at(i).at(j) = temp;
			pixelNum++;
		}
	}
	//checks to make sure the correct number of pixels was inputted
	if (pixelNum != (height*width)) {
		cout << "Incorrect number of pixels.\n";
	}
	fin.close();
	return a;
}

//Picture class function that writes the imageMap vectore of pixels to a new file and puts the header information before all of the pixels. It returns any errors in writing to the file.
int Picture::writeFile(const string &W) {
	int b = 0;
	ofstream fout;
	if (fout.fail()) {
		b = -2;
	}
	if (imageMap.at(0).size() == 0 && imageMap.size() == 0) {
		b = -1;
	}
	fout.open(W);
	fout << "P3" << '\n' << width << " " << height << '\n' << maxInt << '\n';
	int i;
	int j;
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			cout << imageMap.at(i).at(j).red << " " << imageMap.at(i).at(j).green << " " << imageMap.at(i).at(j).blue << '\n';

		}
	}
	return b;
}

//picture class function that flips the image across the y axis
void Picture::yflip() {
	int i;
	int j;
	Pixel temp;
	int end;
	for (i = 0; i < height; i++) {
		end = width;
		for (j = 0; j < end; j++) {
			temp = imageMap.at(i).at(j);
			imageMap.at(i).at(j) = imageMap.at(i).at(end - 1);
			imageMap.at(i).at(end - 1) = temp;
			end--;
		}
	}
}

//picture class function that lfips the image across the x axis
void Picture::xflip() {
	int i; 
	int j;
	Pixel temp;
	int end;
	for (j = 0; j < width; j++) {
		end = height;
		for (i = 0; i < end; i++) {
			temp = imageMap.at(i).at(j);
			imageMap.at(i).at(j) = imageMap.at(end - 1).at(j);
			imageMap.at(end - 1).at(j) = temp;
			end--;
		}
	}
}

//picture class fuction that changes each rgb color to its negative intensity
void Picture::neg(int N) {
	int i;
	int j;
	//switch statement chooses which color(s) to negate based on user specified input
	switch (N) {
		case 1: 
			for (i = 0; i < height; i++) {
				for (j = 0; j < width; j++) {
					imageMap.at(i).at(j).red = maxInt - imageMap.at(i).at(j).red;
				}
			}
			break;
		case 2:
			for (i = 0; i < height; i++) {
				for (j = 0; j < width; j++) {
					imageMap.at(i).at(j).green = maxInt - imageMap.at(i).at(j).green;
				}
			}
			break;
		case 3: 
			for (i = 0; i < height; i++) {
				for (j = 0; j < width; j++) {
					imageMap.at(i).at(j).blue = maxInt - imageMap.at(i).at(j).blue;
				}
			}
			break;
		case 4: 
			for (i = 0; i < height; i++) {
				for (j = 0; j < width; j++) {
					imageMap.at(i).at(j).red = maxInt - imageMap.at(i).at(j).red;
					imageMap.at(i).at(j).green = maxInt - imageMap.at(i).at(j).green;										}
			}
			break;
		case 5: 
			for (i = 0; i < height; i++) {
				for (j = 0; j < width; j++) {
					imageMap.at(i).at(j).red = maxInt - imageMap.at(i).at(j).red;
					imageMap.at(i).at(j).blue = maxInt - imageMap.at(i).at(j).blue;
	            }	
			}
			break;
		case 6: 
			for (i = 0; i < height; i++) {
				for (j = 0; j < width; j++) {
					imageMap.at(i).at(j).green = maxInt - imageMap.at(i).at(j).green;
					imageMap.at(i).at(j).blue = maxInt - imageMap.at(i).at(j).blue;
				}
			}
			break;
		default:	
			break;
	}
}

//picture class function that converts the image to a grayscale by averaging all the values of each color in each pixel individually
void Picture::greys() {
	int i; 
	int j;
	int avgColor;
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			avgColor = (imageMap.at(i).at(j).red + imageMap.at(i).at(j).green + imageMap.at(i).at(j).blue) / 3;
			imageMap.at(i).at(j).red = avgColor;
			imageMap.at(i).at(j).green = avgColor;
			imageMap.at(i).at(j).blue = avgColor;
		}
	}
}
