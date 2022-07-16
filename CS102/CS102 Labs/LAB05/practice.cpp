#include <iostream>
#include <vector>
using namespace std;

int main(){

	unsigned i;
	vector<int> a = {1, 2, 3, 4, 5};
	a.erase( a.begin() + a.size()-1 );
//	a.push_back(17);
//	a[4] = 20;

	for (i=0; i<a.size(); i++){
		cout << a[i] << " ";
	}
	cout << "\n";

	vector<int> b;

	for (i=0; i<a.size(); i++){
		b.push_back(a[i]*a[i]);
		cout << b[i] << " ";
	}
	cout << "\n";

	return 0;

}
