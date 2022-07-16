#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>
using namespace std;

int main() {

   vector<string> vectorName;
   string name;

	cout << "Enter student";
	cin >> name;

		vectorName.push_back(name);
		cout << "Student's name: " << vectorName.at(0);
   return 0;
}
