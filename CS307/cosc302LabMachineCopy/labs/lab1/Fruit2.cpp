
//Joshua Wright cosc302 Gregor Lab 1
//This takes in a file of fruit names, costs, and quantities and optionally formats
//the entries and then outputs the totals for each fruit and the overall total
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdlib.h>

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
	bool operator<=(const fruit &rhs) const {
		if (name != rhs.name) {
			return name < rhs.name;
		}
		else {
			return true;
		}
	}

	bool operator==(const fruit &rhs) const {
		return name == rhs.name;		
	}

	void operator +=(const fruit &rhs) {
		quant += rhs.quant;
	}

	string name;
	double quant;
	double cost;

};

//node struct
struct node {
	node(fruit data);
	fruit data;
	node *next;
};

node::node(fruit n_data) {
	data = n_data;
	next = NULL;
}

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

	//opens file and declares variables
	ifstream fin;
	fin.open(argv[2]);
	vector<fruit> fruits;
	string input;
	istringstream sin;
	fruit empty;
	empty.name = "";
	empty.quant = 0.0;
	empty.cost = 0.0;

	//list head
	node *list = new node(empty);

	//reads in file entries and inserts them into a linked list with optional sorted formatting
	while (getline(fin, input)) {
		fruit temp;
		sin.str(input);
		sin >> temp.name >> temp.quant >> temp.cost;
		sin.clear();
		node *incr = new node(temp);
		node *prev, *p;
		prev = list;
		p = prev->next;

		if (mode == "-inorder") {
			while (p != NULL) {
				prev = p;
				p = prev->next;
			}
			prev->next = incr;
		}
		else if (mode == "-sortall" || mode == "-sortone") {
			bool pres = false;
			if (mode == "-sortone") {
				node *tprev, *tp;
				tprev = prev;
				tp = p;
				while (tp != NULL) {
					if (incr->data == tp->data) {
						pres = true;
						break;
					}
					tprev = tp;
					tp = tprev->next;
				}
				if (pres) {
					tp->data += incr->data;
				}
			}
			if (!pres) {
				while (p != NULL && p->data <= incr->data) {
					prev = p;
					p = prev->next;
				}
				if (p != NULL) {
					incr->next = p;
					prev->next = incr;
				}
				else {
					prev->next = incr;
				}
			}
		}
	}

	fin.close();

	double totalcost = 0.00;

	//prints out linked list of fruites and deallocates linked list memory
	node *c, *n;
	c = list;
	n = list->next;
	while (n != NULL) {
		double ppc = n->data.quant * n->data.cost;
		totalcost += ppc;

		cout << fixed << setprecision(2); 
		cout << left << setw(20) << setfill('.')<< n->data.name << " ";
		cout << setfill(' ');
		cout << right << setw(5) << n->data.quant << " x ";
		cout << right << setw(4) << n->data.cost << " = ";
		cout << right << setw(7) << ppc << "  : ";
		cout << right << setw(8) << totalcost << "\n";

		node *temp = c;
		c = n;
		n = c->next;
		delete temp;
	}

	return 0;
}
