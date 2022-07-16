
//Joshua Wright Cosc140 Lab 7 Greggor 11/16/2020
//This program will take in any number between 2 and 2000 (including but 2 but not 2000) and
//will output all permutations of prime numbers that add up to the inputted number (2 for 
//evens and 3 for odds) in order to demosnstrate goldbachs theory
#include<set>
#include<vector>
#include<iostream>
#include<iterator>
using namespace std;

class prime_partition {
  public:
	prime_partition();
    void operator()(int);

  private:
    void create_pset();
	bool is_prime(int);
	void compute_partitions(vector<int> &, const int &, int = 0);
	void print_partitions();

    set<int> pset;
	unsigned int max_terms;

	vector<vector<int> > partitions;
};

//returns true if prime
bool prime_partition::is_prime(int num) {
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

//populates set pset with all prime numbers from 2 to 2000 (not including 2000)
//for ease of access later
void prime_partition::create_pset() {
	pset.clear();

	for (int i = 2; i < 2000; i++) {
		if (is_prime(i)) {
			pset.insert(i);
		}
	}
}

//constructor
prime_partition::prime_partition() {
	create_pset();
}

//operator overload that checks bounds and calls fucntions to find all added primes
//and then print them out
void prime_partition::operator()(int n) {
	if (n < 2 || n >= 2000) {
		return;
	}
	if (n % 2) {max_terms = 3;}
	else {max_terms = 2;}
	
	partitions.clear();
	vector<int> dummy;

	compute_partitions(dummy, n);
	print_partitions();

}

//prints out list of goldbacks primes
void prime_partition::print_partitions() {
	for (unsigned int i = 0; i < partitions.size(); i++) {
		for (unsigned int j = 0; j < partitions.at(i).size(); j++) {
			cout << partitions.at(i).at(j) << " ";
		}
		cout << "\n";
	}
}

//uses recursion to find all permutations of prime numebers that add up to the
//inputted number (primes listed in descending order)
void prime_partition::compute_partitions(vector<int> &numbers, const int &target, int sum) {
	if (sum == target) {
		if (numbers.size() < max_terms) {
			max_terms = numbers.size();
			partitions.clear();
			partitions.push_back(numbers);
			return;
		}
		else if (numbers.size() == max_terms) {
			partitions.push_back(numbers);
			return;
		}
	}
	if (sum > target || numbers.size() > max_terms) {
		return;
	}
	set<int>::iterator b = pset.begin();
	set<int>::iterator e;
	if (numbers.size() == 0) {
		e = pset.upper_bound(target);
	}	
	else {
		e = pset.upper_bound(numbers.back()); 
	}
	set<int>::iterator ii;
	
	for (ii = b; ii != e; ii++) {
		numbers.push_back(*ii);
		compute_partitions(numbers, target, sum + *ii);
		numbers.pop_back();
	}
}

int main(int argc, char *argv[])
{
  prime_partition goldbach;

  int number;
  while (1) {
	cout << "number = ";
    cin >> number;
	if (cin.eof())
	  break;
	
	goldbach(number);
  }

  cout << "\n";
}
