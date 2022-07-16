#include <iostream>
#include <list>
using namespace std;

template <typename iT, typename T>
int ex_count(iT i1, iT i2, const T &target) {  
	int num = 0;
	for (i1; i1 != i2; ++i1) {   
		if (*i1 == target) {
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
	int N = ex_count(v.begin(), v.end(), target);
	cout << "Found " << N << " instances of " << target << "\n";
}
