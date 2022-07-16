#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

int main(int argc, char*argv[]){ 

	if (argc < 2) {
		cout << "Poop";
	}
	int year;
	int month;
	int day;
	istringstream sin;
	sin.str(argv[1]);
	sin >> year >> month >> day;
	cout << year << " " << (month*-1) << " " << (day*-1);

	
	int yearVal = 2018 * 10000;
	int monthVal = 11 * 100;
	int date = yearVal + monthVal + 27;
	cout << date;
	


	return 0;

	}
