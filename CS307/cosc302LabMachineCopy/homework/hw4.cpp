#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
	if (argc != 3) {
		cout << "usage: " << argv[0]<< " A|B|C"<< " k < numbers.txt\n";
		return 0;
	}
	
	char mode = argv[1][0];
	int k = atoi(argv[2]);
	
	vector<int> A;
	
	int din;
	while (cin >> din) A.push_back(din);
	
	int N = (int)A.size();
	
	if (mode == 'A') {
		partial_sort(A.begin(), A.begin() + k, A.end());
	} 
	else if (mode == 'B') { 
		make_heap(A.begin(), A.end());
		for (int i = 0; i < k; i++) {
			pop_heap(A.begin(), A.end() - i);
		}	
		reverse(A.begin(), A.end());
	} 
	else if (mode == 'C') {
		make_heap(A.begin(), A.end());
		for (int i = 0; i < N; i++) {
			pop_heap(A.begin(), A.end() - i);
		}
	} 
	
	for (int i=0; i<k; i++) cout << A[i] << " *\n";
	
	for (int i=k; i<N; i++) cout << A[i] << "\n";
}
