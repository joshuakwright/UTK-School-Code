#include <iostream>
#include <vector>
using namespace std;

int main(){

	vector<vector<int>> myvector;

	unsigned int row;
	unsigned int col;

	for (row = 0; row < myvector.size(); row++){
		for (col = 0; col < myvector.at(row).size(); col++){
		cout << myvector.at(row).at(col) << ' ';
		}
		cout << '\n';
	}

	
	
	
	
	
	
	
	
	
	
	
	
	
	/*	vector<double> myvector = {1.1, 2.2, 3.3, 4.4};

	myvector.push_back(7.22);

	unsigned int i;
	for (i = 0; i < myvector.size(); i++){
		cout << myvector.at(myvector.size() -1 -i) << "\n";
	}*/

	return 0;
}

