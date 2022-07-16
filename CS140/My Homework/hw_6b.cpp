#include <iostream>
#include <list>
using namespace std;

template<typename T>
class ex_eq {
	public: 
		ex_eq(const T &n_target) {target = n_target;}
		bool operator() (const T &v) const {return v == target;}
		bool operator() (const T &v1, const T &v2) const {return v1 == v2;}

	private:
		T target;
};

template <typename iT, typename T>
int ex_count(iT i1, iT i2, const T &target) {  
	int num = 0;
	ex_eq<int> int_eq_target(target);
	for (i1; i1 != i2; ++i1) {   
		if (int_eq_target(*i1)) {
			num++;
		}
	}

	return num;
}

int main() {
	list<int> v;
	list<int>::iterator iv;
	int value;
	while (cin >> value) {
		v.push_back(value);
	}
	int target = *v.begin();
	int N = ex_count(v.begin(), v.end(), ex_eq<int>(target));
	cout << "Found " << N << " instances of " << target << "\n";
}
