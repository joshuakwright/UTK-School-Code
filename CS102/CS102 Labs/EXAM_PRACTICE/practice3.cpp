#include <vector>
#include <iostream>

using namespace std;

/*The vector v contain letter grades for a student's course load. In here, the vector can contain 
 * A, B, C, D, or F. When finished, return the GPA for that students's course load.
 * Assume each course is 4 credit hours.*/

double calc_gpa(const vector<string> &v);

int main() {

	vector<string> v = {"A", "B", "C", "A", "A", "D", "D"};
	double gpa = calc_gpa(v);
	cout << gpa;

	return 0;
}

double calc_gpa(const vector<string> &v) {
	double ret = 0.0;

	unsigned int i;
	for (i = 0; i < v.size(); i++) {
		string s = v.at(i);
		if (s == "A") {
			ret += 4.0;
		}
		else if (s == "B") {
			ret += 3.0;
		}
		else if (s == "C") {
			ret += 2.0;
		}
		else if (s == "D") {
			ret += 1.0;
		}
	}
		return ret / v.size();

	
}
