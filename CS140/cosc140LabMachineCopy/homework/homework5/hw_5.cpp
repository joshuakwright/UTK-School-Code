#include<iostream>
#include<string>

using namespace std;

template <typename T>
class stats {
	public:
		stats();
		void push(T val);
		void print() const;

	private: 
		int N;
		T sum;
	
};

//instantiates stats' private variables
template <typename T>
stats<T>::stats() {
	N = 0;
	sum = T();
}

template <typename T>
void stats<T>::push(T val) {
	N++;
	sum += val;
}

template <typename T>
void stats<T>::print() const {
	cout << "N   = " << N << "\n" << "sum = " << sum << "\n";
}

int main() {

	int val1;
	float val2;
	string val3;

	stats<int> obj1;
	stats<float> obj2;
	stats<string> obj3;

	int j = 0;
	while (cin >> val1) {
		obj1.push(val1);
		j++;
		if (j == 3) {
			break;
		}
	}
	
	obj1.print();

	int k = 0;
	while (cin >> val2) {
		obj2.push(val2);
		k++;
		if (k == 3) {
			break;
		}
	}

	obj2.print();

	int i = 0;
	while (cin >> val3) {
		obj3.push(val3);
		i++;
		if (i == 3) {
			break;
		}
	}

	obj3.print();

	return 0;
}
