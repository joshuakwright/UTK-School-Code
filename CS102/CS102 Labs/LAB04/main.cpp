//main.cpp
//This lab calculates the summation of the e^x function
//Joshua Wright
//19-Sep-2019
//Professor: Dr.Marz
//Students worked with:
//Dan Ervine

#include <iostream>
#include <limits>
#include <iomanip>
using namespace std;

int main() {

	//Declares variables x and n
	double x;
	int n;

	do {
		//Gets user's initial input
		cout << "Enter x and n: ";
		cin >> x >> n;

		//if statement calculates the summation if the input is valid
		if (!cin.fail() && n > 0){
			double currentSum = 0.0;
		    double interVal = 1.0;
			
		//Calculates summation excluding the first summation at i = 0
			for (int i = 1.0; i <= n; i++){
			interVal = (x / i) * interVal;
			currentSum = currentSum + interVal;
			}
		//adds in the first summation at i = 0
			currentSum = currentSum + 1;
		//prints the summation with 4 decimal places
			cout << "r = " << fixed << setprecision(4) << currentSum << "\n";
			break;
		}
		//clears out the cin if it fails
		else { cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	//do while loop will continue to loop until valid input is entered
	} while (true);

   return 0;
}
