
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

#include "Person.h"
#include "Sptrsort.h"

int main(int argc, char *argv[]) {

	if (argc != 2) {
	   cerr << "usage: ./Data_processor -person | -student | -faculty < data.xml\n";
	   return 0;
	}

	string print = argv[1];
	if (print != "-person" && print != "-student" && print != "-faculty") {
		cerr << "usage: ./Data_processor -person | -student | -faculty < data.xml\n";
		return 0;
	}

	person *n_person;
	vector<person *> person_list;
	vector<string> courses;
	vector<double> gps;
	string name;
	type_enum type = UNKNOWN;
	type_enum category = UNKNOWN;

	int line = 1;
	size_t iL, iR;
	string input;

	while (getline(cin, input)) {
		++line;

		if ((iL = input.find('<')) == string::npos) {
			continue;

		} else if (input.compare(iL,9,"<faculty>") == 0 && (print == "-person" || print == "-faculty")) {
			type = FACULTY;
			courses.clear();

		} else if (input.compare(iL,10,"</faculty>") == 0 && (print == "-person" || print == "-faculty")) {
			n_person = new faculty(name, type, category, courses);
			n_person->print_type = print;
			person_list.push_back(n_person);
			continue;

		} else if (input.compare(iL,9,"<student>") == 0 && (print == "-person" || print == "-student")) {
			type = STUDENT;
			courses.clear();
			gps.clear();

		} else if (input.compare(iL,10,"</student>") == 0 && (print == "-person" || print == "-student")) {
			n_person = new student(name, type, category, courses, gps);
			n_person->print_type = print;
			person_list.push_back(n_person);
			continue;

		} else if (input.compare(iL,5,"<name") == 0) {
			iL = input.find("=\"", iL);
			iR = input.find("\"/>", iL+2);

			char buffer[iR-iL-2];
			input.copy(buffer, iR-iL-2, iL+2); 
			string temp(buffer);
			name = temp;

		} else if (input.compare(iL,10,"<category=") == 0) {
			iL = input.find("=\"", iL);
			iR = input.find("\"/>", iL);

			char buffer[iR-iL-2];
			input.copy(buffer, iR-iL-2, iL+2);
			string subtype(buffer);

			if (subtype.at(0) == 'F') {
				subtype.erase(subtype.begin() + 8, subtype.end());
			}
			if (subtype.at(0) == 'S') {
				subtype.erase(subtype.begin() + 9, subtype.end());
			}

			if (subtype == "Full Professor") { category = FULL_PROF; }
			else if (subtype == "Associate Professor") { category = ASSOC_PROF; }
			else if (subtype == "Assistant Professor") { category = ASST_PROF; }
			else if (subtype == "Freshman") { category = FRESHMAN; }
			else if (subtype == "Sophomore") { category = SOPHOMORE; }
			else if (subtype == "Junior") { category = JUNIOR; }
			else if (subtype == "Senior") { category = SENIOR; }

		} else if (input.compare(iL,7,"<course") == 0) {
			iL = input.find("=\"", iL);
			iR = input.find("\"", iL+2);

			char buffer[iR-iL-2];
			input.copy(buffer, iR-iL-2, iL+2);
			string course(buffer);
			courses.push_back(course);
			
			iL = iR;
			if (type == FACULTY) {
				iR = input.find("/>", iL);
			} else if (type == STUDENT) {
				iL = input.find("gp=\"", iL);
				iR = input.find("\"/>", iL);
					
				char buffer2[iR-iL];
				char buffer3[1];
				string gp2;
				input.copy(buffer2, iR-iL-1, iL+4);
				if (buffer[1] != '.') {
					buffer3[0] = buffer2[0];
					string gp(buffer3);
					gp2 = gp;
				}
				else {
					string gp(buffer2);
					gp2 = gp;
				}
				istringstream sin;
				sin.str(gp2);
				double temp;
				sin >> temp;
				gps.push_back(temp);
			}
		}
	}

	sptrsort(person_list);

	for (int i = 0; i < (int)person_list.size(); i++) {
		if (print == "-person") {
			cout << *person_list[i];
		}
		else if (print == "-student") {
			if (person_list[i]->get_type() == 10) {
				cout << *person_list[i];
			}
		}
		else { 
			if (person_list[i]->get_type() == 0) {
				cout << *person_list[i];
			}
		}
	}

	return 0;

}
