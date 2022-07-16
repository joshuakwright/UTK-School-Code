//Joshua Wright
//Gregor
//1-15-20
//This lab converts decimal to binary and binary to decimal
#include <cctype> 
#include <iostream>
#include <string>
using namespace std;

enum format  { BINARY, DECIMAL, HEXADEC };

/*------------ FUNCTIONS IN LIBNC ------------*/

void printusage();

void dec2bin(string &input, string &output);
void bin2dec(string &input, string &output);
void bin2hex(string &, string &);
void hex2bin(string &, string &);

void processDECIMAL(string &, string &, format);
void processBINARY(string &, string &, format);
void processHEXADEC(string &, string &, format);

/*----------- YOUR IMPLEMENTATIONS -----------*/

void dec2bin(string &input, string &output) {

	//clears output
	for(unsigned int i = 0; i < output.size(); i++) {
		output = "";
	}

	//converts string to decimal
	int number = 0;
	for (unsigned int i = 0; i < input.size(); i++) {
		int temp = 0;
		temp = input[i] - '0';
		number *= 10;
		number += temp;
	}
	
	//converts decimal to binary and places in string
	string bindigit[2] = { "0", "1" };
	while(number != 0) {
		output.insert(0, bindigit[number % 2]);
		number /= 2;
	}
}

void bin2dec(string &input, string &output) {

	//clears output
	for(unsigned int i = 0; i < output.size(); i++) {
		output = "";
	}

	//converts string to decimal
	int number = 0;
	for (unsigned int i = 0; i < input.size(); i++) {
		int temp = 0;
		temp = input[i] - '0';
		number *= 2;
		number += temp;
	}

	//converts binary to decimal and then into a string
	string bindigit[10] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
	while(number != 0) {
		output.insert(0, bindigit[number % 10]);
		number /= 10;
	}
}

/*-------------USER INTERFACE CODE----------------*/

int main(int argc, char *argv[]) {
  string mode;
  string input;
  string output;

  format imode = DECIMAL;
  format omode = BINARY;

  while (1) {
    cout << "enter> ";
    cin >> mode;

	if (cin.eof())
	  break;

	if (mode.compare("dec2bin") == 0) {
      imode = DECIMAL;
      omode = BINARY;
    } else 
	if (mode.compare("dec2hex") == 0) {
      imode = DECIMAL;
      omode = HEXADEC;
    } else 
	if (mode.compare("bin2dec") == 0) {
      imode = BINARY;
      omode = DECIMAL;
    } else 
	if (mode.compare("bin2hex") == 0) {
      imode = BINARY;
      omode = HEXADEC;
    } else 
	if (mode.compare("hex2bin") == 0) {
      imode = HEXADEC;
      omode = BINARY;
    } else 
	if (mode.compare("hex2dec") == 0) {
      imode = HEXADEC;
      omode = DECIMAL;
    } else {
	  cout << mode << " unknown\n";
	  continue;
	}

    cin >> input;

    switch (imode) {
    case DECIMAL:
      processDECIMAL(input, output, omode);
      break;
    case BINARY:
      processBINARY(input, output, omode);
      break;
    case HEXADEC:
      processHEXADEC(input, output, omode);
      break;
    }

	cout << output << "\n";
  }
}
