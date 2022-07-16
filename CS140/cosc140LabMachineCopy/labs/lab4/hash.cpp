
//This program takes in text file and allows you to search it for key words and it returns the line nunbers of that key
//It will also show all of the stats involving the expansion and usage of the hash table it uses
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

 //hash_table class implements a hash table using the key_line to stor the actual data
 class hash_table {

	public:
		hash_table(int N);
		~hash_table();
		void insert(string, int);
		//finds the lines of a given key
		const vector<int> & find(string key) {return table[qprobe(key,false)].line_num;}
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

//hash_table constructor initializes the hash table and accompaningy data memebers
hash_table::hash_table(int N) {
	N = nextprime(N);	
	table.assign(N, key_line());

	num_inuse = 0;
	max_inuse = N/2;
	collisions = 0;
	show_stats = false;
}

//hash_table decosntructor outputs the stats of the hash table usage if the -showstats flag is called
hash_table::~hash_table() {
	if (show_stats) {
		cout << "\n";
		cout << "Run stats ...\n";
		cout << "Number of slots used: " << num_inuse << "\n";
		cout << "Max number of slots: " << max_inuse << "\n";
		cout << "Number of collisions: " << collisions << "\n";
	}
}

//the insert fucntion will input the string key into the hash table and store the 
//line number of that key into its key_line vector and then call the resize funtion if the 
//number of data members excceeds the max allowed for that table size
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
		showload();
		resize();
	}
}

//shows the load on the table every time it is resized
void hash_table::showload() {
	if (show_stats) {
		cout << "** N = " << right << setw(6) << table.size(); 
		double ratio = (double) num_inuse / table.size();
		cout << " load = " << fixed << setprecision(2) << ratio << "\n";
	}
}

//this provides the hash value of the key
int hash_table::hash(string key) {
	uint index = 0;
	const char *c = key.c_str();

	while (*c) {
		index = ((index << 5) | (index >> 27)) + *c++;
	}

	return index % table.size();
}

//this will provide the next largest prime number after double the num inputted
int hash_table::nextprime(int N) {
	N = (N * 2) + 1;
	int i = 2;

	while (i * i <= N) {
		if (N % i == 0) { 
			N++; 
			i = 1; 
		}
		i++;
	}
	return max(3,N);
}

//this will quadratically probe through the current hash table and find the next open spot for a new data element
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

//this will resize the hash table by copying over all the data to a temp vector, resizing the original table,
//and then copying the data from the temp back to the original vector
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

//replaces any punctuation with a space
char replace_punctuation(char c) {
	if (ispunct(c)) {
		return ' ';
	}
	return c;
}

int main(int argc, char *argv[]) {

	//error checks to make sure there is at lest min and less than the max amount of commmand line arguments
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

	//checks the flags and takes action according to which one is present
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
	//shows stats if flag is called
	if (stats)
		words.set_showstats();

	fin.open(inputfile);
	//put all lines in vector for later use
	while (getline(fin, line)) {
		lines.push_back(line);
	}
	fin.close();

	//reads in each line fromt the vector, removes the punctuation, and the inputs each word
	//into the hash table one at a time
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

	//outputs table resize stats
	if (stats) {
		words.insert_done();
	}

	cout << "find> ";

	//allows user to input key and searches the hash table for all the lines that that key is on
	while (cin >> findw) {
		vector<int> outputlines = words.find(findw);
		for (int i = 0; i < outputlines.size(); i++) {
			cout << (outputlines.at(i) + 1) << ":  " << lines.at(outputlines.at(i)) << "\n";
		}
		cout << "find> ";
	}
	cout << endl;

	return 0;
}
