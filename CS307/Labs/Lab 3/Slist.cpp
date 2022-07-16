
//Joshua Wright CS307 Gregor 2/19/21 Lab 3
//This program will take in a list of names and phone numbers and sort them using one of three
//selected sorting algortims and then print them out with the corresponding line number from 
//when they were first inputted
#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

#include "Slist.h"

class data { 
	public:
		//operator overload for comparison
		bool operator<(const data &rhs) const {
			if (rhs.lastname == lastname && rhs.firstname == firstname) {
				return phonenum < rhs.phonenum;
			}
			else if (rhs.lastname == lastname) {
				return firstname < rhs.firstname;
			}
			else {
				return lastname < rhs.lastname;
			}
		}

		//operator overload for custom input of data element
		friend istream & operator>>(istream &, data &);
		friend ostream & operator<<(ostream &, const data &);

	private:
		string firstname;
		string lastname;
		string phonenum;
		int ID;
};

//retrieves data input from in
istream & operator>>(istream &in, data &r) { 
	static int id = 1;
	r.ID = id;
	id++;
	in >> r.firstname >> r.lastname >> r.phonenum;
	return in;
}

//formats data output
ostream & operator<<(ostream &out, const data &r) {
	int fllen = r.lastname.length() +  r.firstname.length();
	out << " " << r.lastname << " " << r.firstname << setw(35 - fllen) << right << r.phonenum;
	out << setw(9) << right << r.ID << endl;
	return out;
}

//prints out sorted data
void printlist(slist<data>::iterator b, slist<data>::iterator e) { 
	while (b != e) {
		cout << *b;
		++b;
	}
}

int main(int argc, char *argv[]) {

	//command line input error checking
	string sorttype;
	if (argc == 3) {
		sorttype = argv[1];
	}
	if (argc != 3) {
		cerr << "Usage: ./Slist -mergesort | -quicksort | -qsort_r3  file.txt\n";
		return 0;
	}
	if (sorttype != "-mergesort" && sorttype != "-quicksort" && sorttype != "-qsort_r3") {
		cerr << "Usage: ./Slist -mergesort | -quicksort | -qsort_r3 file.txt\n";
		return 0;
	}

	//opens file and puts data in vector A
	ifstream fin;
	fin.open(argv[2]);

	slist<data> A;

	data din;
	while (fin >> din) {
		A.push_back(din);
	}

	fin.close();

	A.sort(sorttype);

	printlist(A.begin(), A.end());
}
