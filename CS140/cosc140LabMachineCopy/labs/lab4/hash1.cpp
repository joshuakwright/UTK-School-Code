
#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<iomanip>
#include<fstream>
#include<ctype.h>
#include<sstream>

using namespace std;

 typedef unsigned int uint;

 class hash_table {

	public:
		hash_table(int N);
		~hash_table();
		void insert(string, int);
		const vector<int> & find(string key) {return table[hash(key)].line_num;}
		void insert_done() {showload();}
		void set_showstats() {show_stats = true;}

	private:
		struct key_line {
			string word;
			vector<int> line_num;

			bool inuse() {return !word.empty();}
			bool operator==(const string &str) const {return str == word;}
		};

		void showload();
		int hash(string);
		int nextprime(int);
		int qprobe(string, bool);
		void resize();

		int num_inuse;
		int max_inuse;

		vector<key_line> table;
		int collisions;
		bool show_stats;
 };

hash_table::hash_table(int N) {
	N = nextprime(N);	
	table.assign(N, key_line());

	num_inuse = 0;
	max_inuse = N/2;
	collisions = 0;
	show_stats = false;
}

hash_table::~hash_table() {
	if (show_stats) {
		cout << "\n";
		cout << "Run stats ...\n";
		cout << "Number of slots used: " << num_inuse << "\n";
		cout << "Max number of slots: " << max_inuse << "\n";
		cout << "Number of collisions: " << collisions << "\n";
	}
}
///////////////////////to check	
void hash_table::insert(string key, int iline) {
	int index = qprobe(key, true);

	if (!table[index].inuse()) {
		table[index].word = key;
		num_inuse++;
	}
	if (!(std::find(table[index].line_num.begin(), table[index].line_num.end(), iline) != table[index].line_num.end())) {
		table[index].line_num.push_back(iline);
	}
	if (num_inuse >= max_inuse) {
		resize();
	}
}

void hash_table::showload() {
	cout << "** N = " << right << setw(6) << table.size(); 
	double ratio = num_inuse / table.size();
	cout << " load = " << setprecision(2) << ratio << "\n";
	set_showstats();	
}

int hash_table::hash(string key) {
	uint index = 0;
	const char *c = key.c_str();

	while (*c) {
		index = ((index << 5) | (index >> 27)) + *c++;
	}

	return index % table.size();
}
////////////////////to check
int hash_table::nextprime(int N) {
	N = (N * 2) + 1;
	int i = 2;

	while (i * i <= N) {
		if (N % i == 0) { N++; i = 1; }
		i++;
	}
	return max(3,N);
}
////////////////////////////to check
int hash_table::qprobe(string key, bool col) {
	int index = hash(key);

	int k = 0;
	while (table[index].inuse() && !(table[index] == key)) {
		index += 2 * (++k) - 1;
		index = index % table.size();
		if (col) {
			collisions++;
		}
	}
	return index;
}
///////////////////////////to check
void hash_table::resize() {
	vector<key_line> tmp_table;

	for (int i = 0; i < (int)table.size(); i++) {
		if (table[i].inuse()) {
			tmp_table.push_back(table[i]);
		}
	}

	int N = nextprime(table.size());
	table.assign(N, key_line());

	num_inuse = 0;
	max_inuse = N/2;

	for (int i = 0; i <(int)tmp_table.size(); i++) {
		key_line key = tmp_table[i];
		table[qprobe(key.word, false)] = key;
		num_inuse++;
	}
}

char replace_punctuation(char c) {
	if (ispunct(c)) {
		return ' ';
	}
	return c;
}

int main(int argc, char *argv[]) {

	if (argc < 3 || argc > 6) {
		cout << "usage: ./shashtable [-N number] [-showstats] -f textfile\n";
		return 0;
	}

	int size = 23;
	string inputfile;
	int linenum = 0;
	string line;
	ifstream fin;
	vector<string> lines;
	istringstream sin;
	bool stats = false;
	string findw;

	for (int i = 0; i < argc; i++) {
		string comp = argv[i];
		if (comp == "-N") {
			size = stoi(argv[i + 1]);
		}
		else if (comp == "-showstats") {
			stats = true;
		}
		else if (comp == "-f") {
			inputfile = argv[i + 1];
		}
	}

	hash_table words(size);

	fin.open(inputfile);
	
	while (getline(fin, line)) {
		lines.push_back(line);
	}
	fin.close();

	for (int i = 0; i < lines.size(); i++) {
		string templine = lines.at(i);
		string newline;
		string key;
		newline.resize(templine.size());
		transform(templine.begin(), templine.end(), newline.begin(), replace_punctuation);
		sin.str(newline);
		while (sin >> key) {
			words.insert(key, i);
		}
		sin.clear();
	}

	
/*	
	lines.push_back("hello");
	lines.push_back("me");
	lines.push_back("you");
	lines.push_back("eye");
	lines.push_back("call");
	lines.push_back("he");
	lines.push_back("she");
	lines.push_back("and");
	lines.push_back("how");
	lines.push_back("lead");
	lines.push_back("bug");
	lines.push_back("nick");

	words.insert("hello", 0);
	words.insert("me", 1);
	words.insert("you", 2);
	words.insert("eye", 3);
	words.insert("call", 4);
	words.insert("he", 5);
	words.insert("she", 6);
	words.insert("and", 7);
	words.insert("how", 8);
	words.insert("lead", 9);
	words.insert("bug", 10);
	words.insert("nick", 11);
*/
	if (stats) {
		words.insert_done();
		words.set_showstats();	
	}

	cout << "find> ";

	while (cin >> findw) {
		vector<int> outputlines = words.find(findw);
	//	cout << "hello";
	//	cout << outputlines.size();
		for (int i = 0; i < outputlines.size(); i++) {
			cout << (outputlines.at(i) + 1) << ":  " << lines.at(outputlines.at(i)) << "\n";
		}
		cout << "find> ";
	}
	cout << endl;

	return 0;
}
