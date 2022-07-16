
//Joshua Wright cosc302 Gregor Lab 1
//This takes in a file of fruit names, costs, and quantities and optionally formats
//the entries and then outputs the totals for each fruit and the overall total
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdlib.h>
#include <algorithm>
using namespace std;

//fruit struct
struct fruit {
  bool operator<(const fruit &rhs) const { 
	  if (name != rhs.name) {
		return name < rhs.name; 
	  }
	  else {
		return false;
	  }
  }
  string name;
  double quant;
  double cost;

};

int main(int argc, char *argv[]) {

	//input error checking
	string mode = argv[1];
	if (argc != 3) {
		cerr << "Input Format Error: ./program (-inorder|-sortall|-sortone) file.txt\n";
		return 0;
	}
	if (mode != "-inorder" && mode != "-sortall" && mode != "-sortone") {
		cerr << "Input Format Error: ./program (-inorder|-sortall|-sortone) file.txt\n";
		return 0;
	}
	
	//variable declration and openeing file
	ifstream fin;
	fin.open(argv[2]);
	vector<fruit> fruits;
	string input;
	istringstream sin;

	//takes data from file and puts it in a vector
	while (getline(fin, input)) {
		fruit temp;
		sin.str(input);
		sin >> temp.name >> temp.quant >> temp.cost;
		sin.clear();
		fruits.push_back(temp);
	}

	fin.close();
	
	//optional soritng of vector
	if (mode == "-sortall" || mode == "-sortone") {
		stable_sort(fruits.begin(), fruits.end());
	}
	
	//removes duplicates from vector while preserving total fruit quantity
	if (mode == "-sortone") {
		for (unsigned int i = 0; i < fruits.size() - 1; i++) {
			if (fruits.at(i).name == fruits.at(i + 1).name) {
				fruits.at(i).quant += fruits.at(i + 1).quant;
				fruits.erase(fruits.begin() + i + 1);
				i--;
			}
		}
	}

	double totalcost = 0.00;

	//prints out formatted list of fruits and prices
	for (unsigned int i = 0; i < fruits.size(); i++) {
		double ppc = fruits.at(i).quant * fruits.at(i).cost;
		totalcost += ppc;

		cout << fixed << setprecision(2); 
		cout << left << setw(20) << setfill('.')<< fruits.at(i).name << " ";
		cout << setfill(' ');
		cout << right << setw(5) << fruits.at(i).quant << " x ";
		cout << right << setw(4) << fruits.at(i).cost << " = ";
		cout << right << setw(7) << ppc << "  : ";
		cout << right << setw(8) << totalcost << "\n";
	}

	return 0;
}
