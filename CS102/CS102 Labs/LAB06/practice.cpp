
#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main() {

	vector<string> string;

	string.push_back("Hello");

	int i = string.at(0).length();
	cout << i;

	return 0;
}
