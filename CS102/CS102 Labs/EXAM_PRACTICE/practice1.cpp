#include <iostream>
#include <vector>

using namespace std;

void calc(const vector<double> &v, vector<double> &left, vector<double> &right, double split);

//1. search through vector v
//2. any value of v[x] < split, add that into the left vector
//3. any value of v[x] >= split, add that into the right vector

int main() {

	double split = 10;
	const vector<double> v = {1, 10, 13, 3, 7, -9, 21};
	vector<double> left;
	vector<double> right;

	calc(v, left, right, split);
	for (unsigned int i = 0; i < left.size(); i++) {
		cout << "Left" << left.at(i) << '\n';
	}
	for (unsigned int i = 0; i < right.size(); i++) {
		cout << "Right" << right.at(i) << '\n';
	}
		
	return 0;
}

void calc(const vector<double> &v, vector<double> &left, vector<double> &right, double split) {
	for (unsigned int i = 0; i < v.size(); i++) {
		if (v.at(i) < split) {
			left.push_back(v.at(i));
		}
		else {
			right.push_back(v.at(i));
		}
	}

}
