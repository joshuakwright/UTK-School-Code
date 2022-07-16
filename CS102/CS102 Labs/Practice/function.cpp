#include <iostream>
#include <string>

using namespace std;

string get_name();

int main() {

	string name = gen_name();
	cout << "You entered " << name << '\n';

	return 0;
}

string get_name() {

	string your_name;
	cout << "Enter your name: ";
	cin >> your_name;
	
	return name_name;
}
