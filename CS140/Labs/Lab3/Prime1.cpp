
#include<iostream>

using namespace std;

bool isprime(int num);

int main() {
	
	int num;

	while (cin >> num) {
		if (isprime(num)) {
			cout << num << " is a prime number\n";
		}
	}

	return 0;
}

bool isprime(int num) {

	if (num <= 1)  {return false;}
	if (num <= 3)  {return true;}

	if (num % 2 == 0 || num % 3 == 0) {return false;}

	for (int i = 5; i*i <= num; i = i + 6) {
		if (num % i == 0 || num % (i + 2) == 0) {
			return false; 
		}
	}
    return true;

}
