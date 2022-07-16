include<iostream>
#include<string>
#include<stdlib.h>
#include<cstdlib>

using namespace std;

string encoding(string &line, int rot);
string decoding(string &line, int rot);

int main(int argc, char *argv[]) {

	//argument count error message
	if (argc != 3) {
		cerr << "Error. Format: ./Prog1d -encode/-decode 0-9\n";
		return 0;
	}
	//encode or decode argemnts error message
	if (string(argv[1]) != "-encode" && string(argv[1]) != "-decode") {
		cerr << "Error. Format: ./Prog1d -encode/-decode 0-9\n";
		return 0;
		}
	//loops through allowed numbers to see if input matches on of them
	int j = 0;
	for(int i = 0; i < 10; i++) {
		if (stoi(string(argv[2])) != i) {
			j++;
		}
	}
	//if input doesn't match any allowed numbers, j will be 10 instead of 9 and error message prints
	if (j != 9) {
		cerr << "Error. Format: ./Prog1d -encode/-decode 0-9\n";
		return 0;
	}

	//while loop reads in line, encodes or decodes, then repreats the process until cntrl D is pressed
	string line;
	while(getline(cin, line)) {
		int rot = stoi(string(argv[2]));

		if (string(argv[1]) == "-encode") {
			cout << encoding(line, rot) << "\n";
		}
		else if (string(argv[1]) == "-decode") {
			cout << decoding(line, rot) << "\n";
		}
	}
	return 0;
}


string encoding(string &line, int rot) {

	string eline = line;
	
	//loops through all characters in string, checks if number (which would not change it) then if it is a letter, it checks if it is an upper or lower case and then  it rotates it the input amount forward
	for (int i = 0; eline[i] != '\0'; i++) {
		//t must be int because z + 6 or greater is invalid as a char
		int t = eline[i];
		if (isalpha(t)) {
			if (islower(t)) {
				t += rot;
				if (t > 'z') {
					t-= 26;
				}
			}
			else if (isupper(t)) {
				t += rot;
				if (t > 'Z') {
					t -= 26;
				}
			}
		}
		eline[i] = t;
	}
	
	return eline;
}

string decoding(string &line, int rot) {

	string eline = line;
	//same as encode but it rotates the input amount backwards
    for (int i = 0; eline[i] != '\0'; i++) {
		int t = eline[i];
		if (isalpha(t)) {
			if (islower(t)) {
				t -= rot;
				if (t < 'a') {
					t+= 26;	
				}
			}
			else if (isupper(t)) {
				t -= rot;
				if (t < 'A') {
					t += 26;
				}
			}
		}
		  eline[i] = t;
	}

	    return eline;

}

