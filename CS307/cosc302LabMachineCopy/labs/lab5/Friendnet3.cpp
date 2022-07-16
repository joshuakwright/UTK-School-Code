
//Joshua Wright CS302 Gregor Lab 5
//This program will take in a list of names and create a radomized graph from them and output 
//all their direct friends and indirect friends that are two edges away. So the same as
//Friendnet1&2 but instead of a 2d vector, this program uses a vector of sets (BST).
#include <iostream>
#include <vector>
#include <cstdlib>
#include <set>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <iterator>

using namespace std;

void set_oldfriends(vector<string> &name, vector<set<int> > &friends, int M0, int M1) {
	int N = (int)name.size();
	friends.resize(N);

	set<int> doknow;
	for (int i=0; i<N; i++) {

		int M = M0 + rand() % M1;

		while ((int)doknow.size() < M) {
			int j = i;
			while (j == i) {
				j = rand() % (N);
			}

			doknow.insert(j);
		}

		for (set<int>::iterator it = doknow.begin(); it != doknow.end(); ++it) {
			friends[i].insert(*it);
			friends[*it].insert(i);
		}
		doknow.clear();
	}
}

void set_newfriends(vector<set<int> > &friends, vector<set<int> > &new_friends) {
	int N = (int)friends.size();
	new_friends.resize(N);

	for (int i=0; i<N; i++) {
		set<int>::iterator itj = friends[i].begin();
		for (int j = 0; j < (int)friends[i].size(); j++, itj++) {
			set<int>::iterator itk = friends[*itj].begin();
			for (int k = 0; k < (int)friends[*itj].size(); k++, itk++) {
				bool temp = true;
				for (int n = 0; n < (int)friends[i].size(); n++) {
					set<int>::iterator itin = friends[i].begin();
					for (int o = 0; o < n; o++) {
						itin++;
					}
					if (*itk == *itin) {
						temp = false;
					}
				}
				if (*itk != i && temp) {
					new_friends[i].insert(*itk);
				}
			}
		}
	}
}

void writetofile(const char *fname, vector<string> &name, vector<set<int> > &friends) {

	ofstream fout;
	fout.open(fname);
	if (fout.fail()) {
		cout << "fail\n";
	}

	int N = (int)name.size();

	string temp = name[0];
	for (int i = 1; i < (int)name.size(); i++) {
		if (name[i].size() > temp.size()) {
			temp = name[i];
		}
	}
	int maxlen = (int)temp.size();

	for (int i=0; i<N; i++) {
		fout << setw(maxlen) << left << name[i] << " :";
		int temp = 0;
		for (set<int>::iterator j = friends[i].begin(); j != friends[i].end(); j++) {
			if (temp != 0 && temp % 8 == 0) {
				fout << endl;
				fout << setw(maxlen) << left << name[i] << " :";
			}
			fout << " " << setw(maxlen) << left << name[*j];
			temp++;
		}
		fout << endl;
	}

	fout.close();
}

int main(int argc, char *argv[]) {

	if (argc != 1 && argc != 3) {
		cerr << "usage: ./Friendnet3 [-seed N]\n";
		return 0;
	}

	string seed;
	srand(time(NULL));

	if (argc == 3) {
		seed = argv[1];
		srand(atoi(argv[2]));
		if (seed != "-seed") {
			cerr << "usage: ./Friendnet3 [-seed N]\n";
			return 0;
		}
	}

	vector<string> name;
	string names;
	while (cin >> names) {
		name.push_back(names);
	}

	int M0 = 1; // min number of friends
	int M1 = 3; // max number of friends

	vector<set<int> > friends;
	vector<set<int> > new_friends;

	set_oldfriends(name, friends, M0, M1);
	writetofile("doknow3.txt", name, friends);

	set_newfriends(friends, new_friends);
	writetofile("mightknow3.txt", name, new_friends);

	return 0;
}
