#include<iostream>

using namespace std;

int strlen(char *pointer);

int main(int argc, char *argv[]) {
	
	cout << "Num args = " << argc << "\n";

	int num = argc;
	int temp = argc;
	int len = 0;
	while (temp != 0) {
		int argval = num - temp;
		len = strlen(argv[argval]);
		temp--;

		cout << "argv[" << argval << "] = " << &argv[argval] << " " << argv[argval] << " (strlen=" << len << ")\n";
	}

	return 0;
}

int strlen(char *pointer) {
	int length = 0;

	while ((*pointer) != '\0') {
		pointer++;
		length++;
	}

	return length;
}

