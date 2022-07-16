
//Joshua Wright CS307 Lab 2 Gregor
//This lab will take in a list of first names, last names, and phonenumbers and 
//sort them using either stl::sort or a custom quicksort algorithm using a random pivot.
//It can also optionally sort within a given range. 
#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iterator>
#include <stdlib.h>
#include <time.h>

using namespace std;

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
};

//retrieves data input from in
istream & operator>>(istream &in, data &r) { 
	in >> r.firstname >> r.lastname >> r.phonenum;
	return in;
}

//formats data output
ostream & operator<<(ostream &out, const data &r) {
	int fllen = r.lastname.length() +  r.firstname.length();
	out << r.lastname << ", " << r.firstname << setw(34 - fllen) << right << r.phonenum << "\n";
	return out;
}

//quicksort algorith implemented using a random pivot
template <typename Tdata>
void quicksort(vector<Tdata> &A, int left, int right) { 

	if (right <= left) {
		return;
	}

	int pindex;
	int randpart = rand() % (right - left + 1) + left;

	//select pivot
	pindex = randpart;
	Tdata pivot = A[pindex]; 
	// partition A: {<=}, {pivot}, {=>}   
	swap(A[pindex], A[right]);  
	int i = left - 1;  
	int j = right;   

	while (1) {    
		while (A[++i] < pivot) { }    
		while (pivot < A[--j] && j > i) { }    
		if (i>=j) break;  
		swap(A[i], A[j]);
	}  

	pindex = i;  
	swap(A[pindex], A[right]);  

	quicksort(A, left, pindex - 1);
	quicksort(A, pindex + 1, right);

}

//quickselect causes the inputted number to be put in its correct place and for 
//all greater values to be above it and all lower values to be below it
template <typename Tdata>
void quickselect(vector<Tdata> &A, int left, int k, int right) { 

	while (left < right) {    

		int pindex;
		int randpart = rand() % (right - left + 1) + left;

		//select pivot
		pindex = randpart;
		Tdata pivot = A[pindex]; 
		//partition A: {<=}, {pivot}, {=>}   
		swap(A[pindex], A[right]);  
		int i = left - 1;  
		int j = right;   

		while (1) {    
			while (A[++i] < pivot) { }    
			while (pivot < A[--j] && j > i) { }    
			if (i>=j) break;  
			swap(A[i], A[j]);
		}  

		pindex = i;  
		swap(A[pindex], A[right]);  

		if (pindex == k) { return; }

		if (k < pindex) { right = pindex-1; }   
		else { left = pindex+1; } 
	}
}

//prints out sorted data
void printlist(vector<data>::iterator b, vector<data>::iterator e) { 
	while (b != e) {
		cout << *b;
		b++;
	}
}

int main(int argc, char *argv[]) {

	//command line input error checking
	string sorttype;
	if (argc == 3 || argc == 5) {
		sorttype = argv[1];
	}
	if (argc != 3 && argc != 5) {
		cerr << "Usage: ./Qsort -stl|-rpivot [k0 k1] file.txt\n";
		return 0;
	}
	if (sorttype != "-stl" && sorttype != "-rpivot") {
		cerr << "Usage: ./Qsort -stl|-rpivot [k0 k1] file.txt\n";
		return 0;
	}

	//opens file and puts data in vector A
	ifstream fin;
	if (argc == 3) {
		fin.open(argv[2]);
	}
	else {
		fin.open(argv[4]);
	}

	vector<data> A;

	data din;
	while (fin >> din) {
		A.push_back(din);
	}

	fin.close();

	//optional sorting using c++ stl::sort
	if (sorttype == "-stl") {
		sort(A.begin(), A.end());
	}
	//optional sorting using custom quicksort algorithm or 
	//within a specified range
	else if (sorttype == "-rpivot") {
		int N = (int)A.size();

		int k0 = 0;
		int k1 = N-1;

		if (argc == 5) {
			k0 = atoi(argv[2]);
			k1 = atoi(argv[3]);
			if (k0 < 0) {k0 = 0;}
			if (k1 >= N) {k1 = N - 1;}
			quickselect(A, 0, k0, N-1);
			quickselect(A, k0, k1, N-1);
		}

		quicksort(A, k0, k1);
	}

	printlist(A.begin(), A.end());

	return 0;

}
