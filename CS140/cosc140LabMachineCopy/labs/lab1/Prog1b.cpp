#include<iostream>

using namespace std;

int main() {

	int num;
	int N = 0;
	int sum = 0;
	int min;
	int max;
	cin >> num;
	min, max = num;

	do {
		N++;
		sum += num;
		if (num < min) {
			min = num;
		}
		else if (num > max) {
			max = num;
		}
	} while (cin >> num);

	cout << "N   = " << N << "\n" << "sum = " << sum << "\n" << "min = " << min << "\n" << "max = " << max << "\n";
	
	return 0;
}
