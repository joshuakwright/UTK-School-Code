
//Joshua Wright 11/22/30 Cosc140 Lab8 Jens Greggor
//This program will take in a list of names and scores and the output them and the mean of 
//the scores by mean in descending order
#include<iomanip>
#include<iostream>
#include<vector>
#include<stdlib.h>
#include<map>
#include<string>
#include<fstream>
#include<sstream>
#include<numeric>
#include<algorithm>

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
		float means() const { return mean; }

	private:
		vector<int> scores;
		float mean;
};

class namescores_t {
  public:
    namescores_t();
	namescores_t(const name_t &, const scores_t &);

    bool operator<(const namescores_t &rhs) const;
    
    void print_name(int w = 0) { names.print(w); }
    void print_scores() { scores.print(); }

  private:
    name_t names;
    scores_t scores;
};

name_t::name_t(const string &first, const string &last) {
	ostringstream sout;
	sout << last << ", " << first << " ";
	name = sout.str();
	sout.str("");
}

void name_t::print(int w) const {
	cout << left << setw(w + 3) << setfill('.') << name;
}

void scores_t::insert_done() {
	if (scores.size() == 0) {
		return;
	}
	float sum = accumulate (scores.begin(), scores.end(), 0);
	mean = sum / scores.size();
}

void scores_t::print() {
	cout << right << setfill(' ');
	for (unsigned int i = 0; i < scores.size(); i++) {
		cout << " " << setw(2) << scores[i];
	}
	cout << " : " << fixed << setprecision(1) << mean << "\n";
}

//initializes the names and scores variable
namescores_t::namescores_t(const name_t &n, const scores_t &s) {
	names = n;
	scores = s;
}

//< comparison operator that compares scores, or ny names if the scores are equal
bool namescores_t::operator<(const namescores_t &rhs) const {
	if (scores.means() != rhs.scores.means()) {
		return scores.means() < rhs.scores.means();
	}
	else {
		return !names.operator<(rhs.names);
	}
}

int main(int argc, char *argv[]) {
	
	//w is width of name and k is amount of names to print out
	int w = atoi(argv[1]);
	int k = atoi(argv[2]);

	ifstream fin;
	fin.open(argv[3]);
	vector<namescores_t> NS;
	istringstream sin;
	string line;

	//reads in data from file and uses it to populate vector
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
		namescores_t pair(n, s);
		NS.push_back(pair);

		sin.clear();
	}

	fin.close();

	//creates max heap based off of NS vector
	make_heap(NS.begin(), NS.end());

	//prints out names and scores in sorted order based off mean score by printing 
	//the first element in the heap and then removing that element so the max element
	//is always the first element
	for (int i = 0; i < k; i++) {
		if (NS.size() != 0) {
			NS[0].print_name(w);
			NS[0].print_scores();
			pop_heap(NS.begin(), NS.end());
			NS.pop_back();
		}
	}

	return 0;
}
