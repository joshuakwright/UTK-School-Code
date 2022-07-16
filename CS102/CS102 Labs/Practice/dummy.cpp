#include <iostream>
using namespace std;

int main(){

	char themostimportantanswer;
	cout << "Do you want to die?\n";
	cin >> themostimportantanswer;

	if (themostimportantanswer == 'y'){
		cout << "Understandable. Have a nice day.\n";
	}
	else {
		cout << "You're lying.\n";
	}

	return 0;
}
