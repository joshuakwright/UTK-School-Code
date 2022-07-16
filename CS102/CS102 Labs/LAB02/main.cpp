#include <iostream>
using namespace std;

int main()
{
   // Declare your data types here
	//Variables used in the equation
	float x,y,z;
	float a,b,c,d;
	float r;
	//Variable placeholders for ax^2, by^2, and cz/d
	float e,f;
	int g;

	//Input prompt for x, y, and z variables
	cout << "Enter x, y, z: ";
	cin >> x >> y >> z;
	//Input prompt for a, b, c, and d variable
	cout << "Enter a, b, c, d: ";
	cin >> a >> b >> c >> d;

   // Perform the math operations here
	//Performs the ax^3 operation
	e = a * (x * x * x);
	//Performs the by^2 operation
	f = b * (y * y);
	//Performs the cz/d operation
	g = (c * z) / d;
	/*Uses the three components of the equation to compute the total
	 *value of r */
	r = e + f - g;
   
   // Finish the following to print the value of r.
   cout << "r = " << r << '\n';

	return 0;
}
