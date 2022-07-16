#include<iostream>

using namespace std;

int main(int argc, char *argv[]) {
	
	cout << "Num args = " << argc << "\n";

	int num = argc;
	int temp = argc;
	while (temp != 0) {
		int argval = num - temp;
		temp--;

		cout << "argv[" << argval << "] = " << &argv[argval] << " " << argv[argval] << "\n";
	}

	return 0;
}
