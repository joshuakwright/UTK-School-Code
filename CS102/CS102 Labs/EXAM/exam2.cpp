#include <vector>
#include <iostream>
using namespace std;
vector<double> MultVector(const vector<vector<double>> &m, const vector<double> &v);

int main() {

	vector<vector<double>> m = {{1, 2, 3, 4}, {5, 6, 7, 8}, {1, 2, 3, 4}, {5, 6, 7, 8}};
	vector<double> v = {1.0, 2.0, 3.0, 4.0};

	vector<double> output;
	output = MultVector(m, v);
	for (unsigned int i = 0; i < output.size(); i++){ 
		cout << output.at(i) << '\n';
	}

	return 0;

}

vector<double> MultVector(const vector<vector<double>> &m, const vector<double> &v) {
	
	vector<double> multiples;
	double sum = 0;
	for (unsigned int i = 0; i < v.size(); i++) {
		for (unsigned int j = 0; j < m.at(i).size(); j++) {
			sum = sum + v.at(i) * m.at(i).at(j);		
		}
		multiples.push_back(sum);
		sum = 0;
	}
	return multiples;
}
