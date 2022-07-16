#include <fstream>
#include <string>
#include <vector>
using namespace std;

bool is_in(int list[], int size, int key);

vector<string> ReadFile(ifstream &fin, int skip_list[], int num_skips);

int main() {

	   /* Type your code here. */

	   return 0;
}

bool is_in(int list[], int size, int key) {
	   bool b = false;
	     // for loop that iteraties throught the list and compares it to the key value
		for (int i = 0; i < size; i++) {
		if (list[i] == key) {
			b = true;
		}
	}
		  return b;
}
		  
vector<string> ReadFile(ifstream &fin, int skip_list[], int num_skips) {
	string line;
	vector<string> lines;
	int j = 0;
	//while loop that reads in the lines until there is no more lines
	while (getline(fin, line)) {
		bool a;
		++j;
		//loop that checks to see if the current line is on the skip list
		for (int i = 0; i < num_skips; i++) {
		if (j == skip_list[i]) {
		  a = false;
		  break;
		}
		else {
			a = true;
		}
	}                                                                                                                                             
		//if the line is not on the skip list and doesn't start with #, it adds it to the vector
		if (!(line.at(0) == '#') && a) {
		  lines.push_back(line);  
		}
		  fin.clear();
	}
		  return lines;
}
