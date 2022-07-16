
#include<iostream>
#include<iomanip>
#include<cstdlib>

using namespace std;

#include "Person.h"

bool person::operator<(const person &rhs) const {
	if (print_type == "-person") {
		if (type != rhs.type) {
			return type < rhs.type;
		}
		else {
			return name < rhs.name;
		}
	}
	else {
		if (category != rhs.category) {
			return category < rhs.category;
		}
		else {
			return name < rhs.name;
		}
	}
}

ostream & operator<<(ostream &out, person &n_person) {
	n_person.print_personinfo(out);
	n_person.print_courseinfo(out);
	return out;
}

student::student(string _name, type_enum _type, type_enum  _category, vector<string> _courselist, vector<double> _gp) {
	name = _name;
	type = _type;
	category = _category;
	courselist = _courselist;
	gp = _gp;
}

void student::print_personinfo(ostream &out) {
	out << right << setw(10) << "Name: " << name << endl;
	out << right << setw(10) << "Category: ";
	switch (category) {
		case 11:
			out << "Freshman" << endl;
			break;
		case 12: 
			out << "Sophmore" << endl;
			break;
		case 13: 
			out << "Junior" << endl;
			break;
		case 14: 
			out << "Senior" << endl;
			break;
		default: 
			break;
	}
}

void student::print_courseinfo(ostream &out) {
	double avggp = gp[0];
	double total = gp[0];
	for (int i = 0; i < (int)courselist.size(); i++) {
		out << right << setw(10) << "Course: ";
		out << left << setw(26) << courselist[i];
		out << fixed << setprecision(2) << gp[i] << " " << avggp;

		if ((i + 1) < (int)courselist.size()) { 
			total += gp[i+1];
			avggp = total / (i+2);
		}
		out << endl;
	}
	out << endl;
}

faculty::faculty(string _name, type_enum _type, type_enum _category, vector<string> _courselist) {
	name = _name;
	type = _type;
	category = _category;
	courselist = _courselist;
}

void faculty::print_personinfo(ostream &out) {
	out << right << setw(10) << "Name: " << name << endl;
	out << right << setw(10) << "Category: ";
	switch (category) {
		case 1:
			out << "Assistant Professor" << endl;
			break;
		case 2: 
			out << "Associate Professor" << endl;
			break;
		case 3: 
			out << "Full Professor" << endl;
			break;
		default: 
			break;
	}
}

void faculty::print_courseinfo(ostream &out) {
for (int i = 0; i < (int)courselist.size(); i++) {
		out << right << setw(10) << "Course: " << courselist[i];
		out << endl;
	}
	out << endl;
}
