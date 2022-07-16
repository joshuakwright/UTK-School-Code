
//Joshua Wright 11/22/2020 Cosc14- Jens Greggor Lab 8
//This program will take in a list of names and scores and then output the names, scores, and mean scores
//in descending order by last name
#include<iomanip>
#include<iostream>
#include<vector>
#include<stdlib.h>
#include<map>
#include<string>
#include<fstream>
#include<sstream>
#include<numeric>

using namespace std;

class name_t {
  public:
    name_t() {name = "";}
	name_t(const string &, const string &);

    bool operator<(const name_t &rhs) const { return name < rhs.name; }
    void print(int w = 0) const;

  private:
    string name;
};

class scores_t {
  public:
    scores_t() { mean = 0.0; }

    void insert(int num) { scores.push_back(num); }
    void insert_done();
    void print();

  private:
    vector<int> scores;
    float mean;
};

//initializes the name in name_t with a string containing both first and last name
name_t::name_t(const string &first, const string &last) {
	ostringstream sout;
	sout << last << ", " << first << " ";
	name = sout.str();
	sout.str("");
}

//formats and prints names
void name_t::print(int w) const {
	cout << left << setw(w + 3) << setfill('.') << name;
}

//calculates mean score for one person
void scores_t::insert_done() {
	if (scores.size() == 0) {
		return;
	}
	float sum = accumulate (scores.begin(), scores.end(), 0);
	mean = sum / scores.size();
}

//formats and prints the scores, followed by the mean
void scores_t::print() {
	cout << right << setfill(' ');
	for (unsigned int i = 0; i < scores.size(); i++) {
		cout << " " << setw(2) << scores[i];
	}
	cout << " : " << fixed << setprecision(1) << mean << "\n";
}

int main(int argc, char *argv[]) {
	int w = atoi(argv[1]);

	ifstream fin;
	fin.open(argv[2]);
	map<name_t,scores_t> NS;
	istringstream sin;
	string line;

	//extracts name and scores from each line and uses them to pupulate a map
	while (getline(fin, line)) {
		string first, last;
		sin.str(line);
		sin >> first >> last;
		name_t n(first, last);

		int num;
		scores_t s;
		while (sin >> num) {
			s.insert(num);
		}
		
		s.insert_done();
		if (NS.find(n) == NS.end()) {
			NS.insert(make_pair(n, s));
		}
		sin.clear();
	}

	fin.close();

	//prints out names and scores in sorted order
	map<name_t, scores_t>::iterator it = NS.begin();
	while (it != NS.end()) {
		it->first.print(w);
		it->second.print();
		it++;
	}
	return 0;
}
