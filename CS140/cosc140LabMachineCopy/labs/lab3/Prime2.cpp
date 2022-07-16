
#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

class isprime {

	public:
		isprime();
		bool operator() (int num);

	private:
		void magic(int num);
		bool is_prime(int num);

		vector<int> cache;

};

//initializes 2 and 3 because that is required by prime algorithm used
isprime::isprime() {
	cache.push_back(2);
	cache.push_back(3);
}

//opertor () overload that calls magic funciton to add prime number to the list and the searches the list to see if the number is on there, if it is it is prime and returns true
bool isprime::operator() (int num) {
	magic(num);
	return find(cache.begin(), cache.end(), num) != cache.end();
}

//magic function adds all prime numbers leading up to and including the number inputted and not already on the list a
void isprime::magic(int num) {
	for (int i = (cache.back() + 1); i <= num; i++) {
		if (is_prime(i)) {
			cache.push_back(i);
		}
	}
}

//uses optomised school method for prime numbers to see if inputted num is prime and returns true if it is
bool isprime::is_prime(int num) {

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

 int main() {

	isprime numcheck;
	int num;

	while (cin >> num) {
		if (numcheck(num)) {
			cout << num << " is a prime number\n";
		}
	}

	return 0;
 }

