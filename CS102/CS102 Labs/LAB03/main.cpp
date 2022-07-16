//main.cpp
//This lab calculates addition, subtraction, multiplication, and division.
//Joshua Wright
//6-Sep-2019
//Professor: Marz
//Students worked with:
//Dan Ervin

#include <iostream>
#include <iomanip>
using namespace std;

int main() {
	
	// Variables set for left, right and operand.
	double leftop;
	double rightop;
	char operand;
   
	// Takes is left operand and outputs error message if invalid.
	cout << "Enter left operand: ";
	if (!(cin >> leftop)){
		cout << "Invalid left operand.\n";
		return 0;
	}

	// Takes in right operand adn outputs error message if invalid.
	cout << "Enter right operand: ";
	if (!(cin >> rightop)){
		cout << "Invalid right operand.\n";
		return 0;
	}

	// Takes in operator.
	cout << "Enter operation: ";
	cin >> operand;

	// Switch statement does calculations between left and right operand.
	switch (operand){
	
	// Addition operator
	case 'A':
	case 'a':
	case '+':
			cout << fixed << setprecision(2) << "Result = " <<  leftop + rightop << "\n";
			break;
	
	// Subtraction operator
	case 's':
	case 'S':
	case '-':
			cout << fixed << setprecision(2) << "Result = " <<  leftop - rightop << "\n";
			break;

	// Multiplication operator
	case 'M':
	case 'm':
	case 'x':
	case '*':
			cout << fixed << setprecision(2) << "Result = " <<  leftop * rightop << "\n";
			break;

	// Division operator
	case 'D':
	case 'd':
	case '/':
			cout << fixed << setprecision(2) << "Result = " << leftop / rightop << "\n";
			break;
	
	// Outputs error if unknown operator is entered.
	default: 
		cout << "Unknown operation.\n";
			break;
	}

   return 0;

}
