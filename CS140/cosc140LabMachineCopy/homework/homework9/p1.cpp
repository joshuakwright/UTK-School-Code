#include<iostream>

using namespace std;

void magic(const char *c) {
	if (!(*c)) {
		return;
	}
	magic(c+1);
	cout << *c;
}

int main(int argc, char *argv[]) {
	
	char text[80] = "purz odwhp bp hwd ozr up pdujhohw vkdudv hhv kdurq vwho hjudp";
//	while (cin.getline(text, 80)) {
		magic(text);
		cout << "\n";
//	}

	return 0;
}
