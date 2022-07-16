
#include<string>
#include<iostream>
#include<vector>
#include<algorithm>
#include<cstdlib>
#include<iomanip>

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
	return binary_search(cache.begin(), cache.end(), num);
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

//generates a random number between and including 1 and 140
int random_number() {
	return (rand() % 140) + 1;
}

//extracts all of the prime numbers form randnums and places them in exprimes
void extract_prime(vector<int> &randnums, vector<bool> &bprimes, vector<int> &exprimes) {
	int j = 0;
	for (int i = 0; i < randnums.size(); i++) {
		if (bprimes.at(i)) {
			exprimes.at(j) = randnums.at(i);
			j++;
		}
	}
}

//prints out contents of outs vector with a width of 4 and 20 numbers to a line
void print(vector<int> outs) {
	for (int i = 1; i <= outs.size(); i++) {
		cout << right << setw(4) << outs.at(i - 1);
		if ((i % 20 == 0) && (i != 0)) {
			cout << "\n";
		}
	}
	cout << "\n";

}

 int main(int argc, char *argv[]) {

	isprime numcheck;
	vector<int> numbers;
	vector<bool> primes;
	vector<int> fprimes;
	int N = 140;
	int tprimes;

	//seeds srand with input number or defualt 140 if none
	if (argc == 2) {
		N = stoi(argv[1]);
		srand(N);
	}
	else {
		srand(N);
	}

	numbers.resize(N);
	primes.resize(N);
	
	//populates numbers with random numbers
	generate(numbers.begin(), numbers.end(), random_number);

	//fills primes true for every corrresponeding prime number in numbers
	transform(numbers.begin(), numbers.end(), primes.begin(), numcheck);

	//counts the number of primes found
	tprimes = count(primes.begin(), primes.end(), 1);

	cout << "Sequence contains " << tprimes << " prime numbers.\n";

	fprimes.resize(tprimes);
	extract_prime(numbers, primes, fprimes);
	cout << "All numbers in order appearance:\n";
	print(fprimes);

	//sorts prime list, puts all the unique values at the front of the vector, then erases the duplicates before printing
	sort(fprimes.begin(), fprimes.end());
	vector<int>::iterator it = unique(fprimes.begin(), fprimes.end());
	fprimes.erase(it, fprimes.end());
	cout << "Unique values in numerical order:\n";
	print(fprimes);

	return 0;
 }

