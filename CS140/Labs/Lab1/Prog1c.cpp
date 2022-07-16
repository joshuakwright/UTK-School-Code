#include<iostream>

using namespace std;

class stats {
	public:
		stats();
		void push(int num);
		void print() const;

	private: 
		int N, sum, min, max;
	
};

//instantiates stats' private variables
stats::stats() {
	N = 0;
	sum = 0;
	min = 0;
	max = 0;
}

void stats::push(int num) {
	if (min == 0) {
		min = num;
		max = num;
	}
	N++;
	sum += num;
	if (num < min) {
		min = num;
	}
	else if (num > max) {
	max = num;
	}
}

void stats:: print() const {
	cout << "N   = " << N << "\n" << "sum = " << sum << "\n" << "min = " << min << "\n" << "max = " << max << "\n";
}

int main() {

	int num;
	stats obj;

	while (cin >> num) {
		obj.push(num);
	}
	
	obj.print();

	return 0;
}
