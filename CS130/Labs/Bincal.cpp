
#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

typedef unsigned char byte; 

byte msb(byte b);
bool overflow(byte x, byte y, byte z);
byte twoscomplement(byte b);

byte str2bin(string &str);
string bin2str(byte b); 
string bin2dec(byte);
bool isBIN8(string &str);
bool isVALID(string &str);

int main(int argc, char *argv[]) {
	string sX, sY;
	string op;

	cout << "X>  "; 
	cin >> sX;
	if (cin.eof())
	  return 1;

	if (!isBIN8(sX)) {
	  cout << "error: not 8-bit binary number\n";
	  return 1;
	}

	cout << "Y>  "; 
	cin >> sY;
	if (cin.eof())
	  return 1;

	if (!isBIN8(sY)) {
	  cout << "error: not 8-bit binary number\n";
	  return 1;
	}

	cout << "op> "; 
	cin >> op;
	if (cin.eof())
	  return 1;

	if (!isVALID(op)) {
	  cout << "error: unknown operation\n";
	  return 1;
	}

	// Done reading input, now execute

	byte X = str2bin(sX);
	byte Y = str2bin(sY);

	if (op.compare("-") == 0)
	  Y = twoscomplement(Y);

	byte S = X + Y;
	bool V = overflow(X, Y, S);
    
	cout << "\n";

    cout << "    "
	     << setw(8) << right << bin2str(X)
	     << setw(6) << right << bin2dec(X) << "\n";

    cout << "  " << op << " "
	     << setw(8) << right << bin2str(Y)
	     << setw(6) << right << bin2dec(Y) << "\n";

    cout << "    "
	     << setw(8) << right << bin2str(S)
	     << setw(6) << right << bin2dec(S) << "\n";

	if (V)
	  cout << "\n** overflow occurred\n";

	cout << "\n";
    
	return 0;
}

byte msb(byte b) {
	byte a;
	a = b >> 7;
	return a;
}

bool overflow(byte x, byte y, byte z) {
	byte bx = msb(x);
	byte by = msb(y);
	byte bz = msb(z);
	bool t;
	if (((~bx&~by&bz)|(bx&by&~bz))==1) {
		t = true;
	}
	else {
		t = false;
	}
	return t;
}

byte twoscomplement(byte b) {
	return (~b) + 1;
}

byte str2bin(string &str) {
	byte a;
	for (unsigned int i = 0; i < str.length(); i++) { 
		a <<= 1;
		a = ((str[i] - '0') | a);
	}
	return a;
}

string bin2str(byte b) {
	string fin;
	string samp = "01";
	for (int i = 0; i < 8; ++i) {
		byte a = b >> i;
		fin = samp.at(a & 1) + fin;
	}
	return fin;
}

bool isBIN8(string &str) {
	if ((str.length() >= 1) || (str.length() <= 8)) {
		return true;
	}

	for (unsigned int i = 0; i < str.length(); i++) {
		if ((str.at(i) == 0) || (str.at(i) == 1)) {
			return true;
		}
	}
	return false;
}

bool isVALID(string &str) {
	if ((str == "+") || (str == "-")) {
		return true;
	}
	else {
		return false;
	}
}
