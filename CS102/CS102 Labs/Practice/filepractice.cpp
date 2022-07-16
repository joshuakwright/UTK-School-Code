#include<iostream>
#include<fstream>
#include<string>
#include<vector>
using namespace std;

int main(){

	ifstream fin;
	vector<int> ages;
	vector<string> first;
	vector<string> last;
	string first;
	string last;
	int age;

	fin.open("text.txt");
	if (fin.fail()){
		cout << "File not found.";
		return -1;
	}

	while (fin >> first >> last >> age){
		first.push_back(first);
		last.push_back(last);
		ages.push_back(age);
	}

	fin.close();

	ofstream fout;

	fout.open("text.redo.txt");
	if (fout.fail()){
	cout << "Could not open text.redo.txt for writing.\n";
	return -1;
	}
	for (unsigned int i = 0; i < first.size(); i++){
		fout << first.at(first.size() - i - 1)
			<< " " << last.at(last.size() - i - 1)
			<< " " << age.at(age.size() - i - 1);
	}
	fout.close();
	exit(0);

	return 0;
}
