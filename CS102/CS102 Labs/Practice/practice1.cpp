#include <iostream>
#include <iomanip>
using namespace std;
int main()
{
	
	int theint;
	double thedouble;
	long thelong;
	string thestring;

	cin >> theint >> thedouble >> thelong >> thestring;

	cout << left << setw(10) << theint << '\n';
	cout << right << fixed << setprecision(3) << setw(15) << thedouble << '\n';
	cout << left << setw(5) << setfill('.') << thelong << '\n';
	cout << right << setw(10) << thestring << '\n';

	return 0;

} 
