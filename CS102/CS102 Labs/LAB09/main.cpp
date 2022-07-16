//main.cpp
//This lab reads in a list of integers from a file and sorts them into ascending order based on a user specified sorting algorithim.
//Joshua Wright
//6-Nov-2019
//Professor: Dr.Marz
//Students worked with:
//Dan Ervine
#include <iomanip>
#include <vector>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <fstream>

using namespace std;

//sorting function prototypes
void selectionSort(vector<int> &numbers);
void insertionSort(vector<int> &numbers);
void bubbleSort(vector<int> &numbers);

int main(int argc, char*argv[]) {
	//error checks for enough input parameters
	if (argc < 2) {
		cout << "Usage: ./a.out <filename> [sort type s|i|b]\n";
		return 0;
	}
	//opens file and outputs error if there is one
	ifstream fin;
	fin.open(argv[1]);
	if (fin.fail()) {
		perror(argv[1]);
		return 0;
	}
	//variable declration
	vector<int> vtemp;
	string stemp;
	int itemp;
	istringstream sin;
	//reads in lines from the file, turns them into itegers, and puts them in a vector
	while (getline(fin, stemp)) {
		sin.str(stemp);
		sin >> itemp;
		vtemp.push_back(itemp);
		sin.clear();
	}
	//chooses sorting type based off user input
	char ctemp;
	if (argc == 3) {
	sin.str(argv[2]);
	sin >> ctemp;
	sin.clear();
	}
	else if (argc == 2) {
		ctemp = 'e';
	}
	
	if (ctemp == 's' || ctemp == 'i' || ctemp == 'b' || ctemp == 'e') {
		switch (ctemp) {
			case 's': 
				selectionSort(vtemp);
			break;
			case 'i':
				insertionSort(vtemp);
			break;
			case 'b':
				bubbleSort(vtemp);
			break;
			case 'e':
				selectionSort(vtemp);
			break;
		}
	} //error checks user input for sorting type
	else {
		cout << "Unknown sorting specified '" << ctemp << "'.\n";
		return 0;
	}
	//once the sorted list is recieved, this outputs all the values in the specified format
	unsigned int i;
	for (i = 0; i < vtemp.size(); i++) {
		cout << right << setw(7) << vtemp.at(i);
		if (((i + 1) % 8) != 0) {
			cout << " ";
		}
		if (((i + 1) % 8) == 0) {
			cout << '\n';
		}
	}
	if (((i + 1) % 8) != 0) {
		cout << '\n';
	}

	fin.close();
	return 0;
}
//selection sort sorting algorithim
void selectionSort(vector<int> &numbers) {
	unsigned int i;
	unsigned int n = 0;
	//takes in vector of numbers, finds the smallest value, places that value at the beginning, and then does the same thing again, excluding the new smallest value until the whole list is sorted
	for (i = n; i < numbers.size(); i++) {
		unsigned int mn = i;
		for (unsigned int j = i + 1; j < numbers.size(); j++) {
			if (numbers.at(j) < numbers.at(mn)) {
				mn = j;
			}
		}
		int temp = numbers.at(i);
		numbers.at(i) = numbers.at(mn);
		numbers.at(mn) = temp;
		n++;
	}
}
//insertion sort sorting algorithim
void insertionSort(vector<int> &numbers) {
	unsigned int i;
	unsigned int j;
	//this starts by comparing the first two numbers and moving the larger value up, and then comparing the next set, and so on, gradually moving the smallest number to the beginning each time, and then taking off the new first value each time until the entire list is sorted and swapped ends up being false, ending the loop
	for (i = 1; i < numbers.size(); i++) {
		j = i;

		while (j > 0 && numbers.at(j) < numbers.at(j - 1)) {
			int temp = numbers.at(j);
			numbers.at(j) = numbers.at(j - 1);
			numbers.at(j - 1) = temp;
			j--;
		}
	}
}
//bubble sort sorting algorithim
void bubbleSort(vector<int> &numbers) {
	bool swapped;
	unsigned int n = numbers.size();
	//this function moves the largest value to the top of the list each interation and then doesn't evaluate the largest value after every interation and does this until n = 1
	do {
		swapped = false;
		for (unsigned int i = 1; i < n; i++) {
			if (numbers.at(i - 1) > numbers.at(i)) {
				int temp = numbers.at(i - 1);
				numbers.at(i - 1) = numbers.at(i);
				numbers.at(i) = temp;
				swapped = true;
			}
		}
		n--;
	} while(swapped);
}
