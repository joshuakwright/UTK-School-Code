
//Joshua Wright CS302 Gregor Lab 5
//This program will take in a list of names and create a radomized graph from them and output 
//all their direct friends and indirect friends that are two edges away. In this program
//the graph is stored in a dense 2d matrix.
#include <iostream>
#include <vector>
#include <cstdlib>
#include <set>
#include <fstream>
#include <iomanip>
#include <algorithm>

using namespace std;

//this function populates the 2d vector be randomly selecting between and including
//1 and 3 friends. But the graph is symetrical so people can have more than three friends
void set_oldfriends(vector<string> &name, vector<vector<int> > &friends, int M0, int M1) {
	int N = (int)name.size();
	friends.assign(N, vector<int>(N,-1));

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

		int j = 0;
		while (friends[i][j] != -1) {
			j++;
		}
		for (set<int>::iterator it = doknow.begin(); it != doknow.end(); ++it) {
			friends[i][j] = *it;
			int k = 0;
			while (friends[*it][k] != -1) {
				k++;
			}
			friends[*it][k] = i;
			j++;
		}
		doknow.clear();
	}
}

//this populates the 2d vector new_friends with all the people that are two edges 
//away from each person
void set_newfriends(vector<vector<int> > &friends, vector<vector<int> > &new_friends) {
	int N = (int)friends.size();
	new_friends.assign(N, vector<int>(N,-1));

	for (int i=0; i<N; i++) {
		int m = 0;
		for (int j = 0; j < (int)friends[i].size(); j++) {
			if (friends[i][j] != -1) {
				for (int k = 0; k < (int)friends[friends[i][j]].size(); k++) {
					if (friends[friends[i][j]][k] != -1) {
						bool temp = true;
						for (int n = 0; n < (int)friends[i].size(); n++) {
							if (friends[friends[i][j]][k] == friends[i][n] && friends[friends[i][j]][k] != -1 && friends[i][n] != -1) {
								temp = false;
							}
						}
						if (friends[friends[i][j]][k] != i && temp) {
							new_friends[i][m] = friends[friends[i][j]][k];
							m++;
						}
					}
				}
			}
		}
	}
}

//this outputs the inputted graph in a formatted style showing each person and
//who they are friends with
void writetofile(const char *fname, vector<string> &name, vector<vector<int> > &friends) {

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

	//this for loop sorts and removes unique elements as well as resizing the vector
	//after unique is called
	for (int i = 0; i < N; i++) {
		int temp = 0;
		for (int j = 0; j < (int)friends[i].size(); j++) {
			if (friends[i][j] != -1) {
				temp++;
			}
		}
		sort(friends[i].begin(), friends[i].begin() + temp);
		vector<int>::iterator ite = friends[i].end();
		vector<int>::iterator itne;
		itne = unique(friends[i].begin(), friends[i].end());
		temp = 0;
		for (; itne != ite; itne++) {
			temp++;
		}
		friends[i].resize(friends[i].size() - temp);
	}

	for (int i=0; i<N; i++) {
		fout << setw(maxlen) << left << name[i] << " :";
		for (int j = 0; j < (int)friends[i].size(); j++) {
			if (friends[i][j] != -1) {
				if (j != 0 && j % 8 == 0) {
					fout << endl;
					fout << setw(maxlen) << left << name[i] << " :";
				}
				fout << " " << setw(maxlen) << left << name[friends[i][j]];
			}
		}
		fout << endl;
	}

	fout.close();
}

int main(int argc, char *argv[]) {

	if (argc != 1 && argc != 3) {
		cerr << "usage: ./Friendnet1 [-seed N]\n";
		return 0;
	}

	string seed;
	srand(time(NULL));

	if (argc == 3) {
		seed = argv[1];
		srand(atoi(argv[2]));
		if (seed != "-seed") {
			cerr << "usage: ./Friendnet1 [-seed N]\n";
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

	vector<vector<int> > friends;
	vector<vector<int> > new_friends;

	set_oldfriends(name, friends, M0, M1);
	writetofile("doknow1.txt", name, friends);

	set_newfriends(friends, new_friends);
	writetofile("mightknow1.txt", name, new_friends);

	return 0;
}
