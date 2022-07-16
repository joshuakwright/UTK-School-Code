
//Joshua Wright Lab 8 CS307 Greggor
//This is a simple implemenetation of diff that takes in two text files and computes the longest common 
//subsequence and outputs the changes needed to make them the same
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

template <typename T>
class matrix {
	public:
		matrix() { data = NULL; }

		~matrix() {
			if (data) {
				if (data[0]) {
					delete [] data[0];
				}
				delete [] data;

				data = NULL;
			}
		}

		//creates 2d array
		void assign (int _Nrows, int _Ncols, const T &value=T()) {
			Nrows = _Nrows;
			Ncols = _Ncols;

			data = new T *[Nrows];
			data[0] = new T [Nrows*Ncols];
			for(int i=1; i<Nrows; i++) {
				data[i] = data[i-1] + Ncols;
			}

			for(int i = 0; i<Nrows; i++) {
				for(int j = 0; j<Ncols; j++) {
					data[i][j] = value;
				}
			}
		}

		int get_Nrows() const {return Nrows;}
		int get_Ncols() const {return Ncols;}

		T * operator[] (int i) {return data[i];}

	private:
		int Nrows, Ncols;
		T **data;
};

class LCS {
	public:
		LCS();
		
		void text1_push_back(string);
		void text2_push_back(string);
		void compute_alignment();
		void report_difference();

	private:
		int m, n;

		vector<string> text1;
		vector<string> text2;

		vector<int> moves;

		matrix<int> cost; //edit costs
		matrix<int> link; //alignmnet info

		const int VERT;
		const int HORZ;
		const int DIAG;

		int (LCS::*DEL)(string);
		int (LCS::*INS)(string);
		int (LCS::*SUB)(string, string);

		int DELcost(string c) { return (*this.*DEL)(c); }
		int INScost(string c) { return (*this.*DEL)(c); }
		int SUBcost(string c1, string c2) { return (*this.*SUB)(c1, c2); }

		//Longest common subsequence cost funciton (no subs)
		int DEL2(string c) {return 1;}
		int INS2(string c) {return 1;}
		int SUB2(string c1, string c2) {return c1==c2 ? 0: m+n;}

		void report_difference(int, int, int);
		void report_difference(int, int, int, int, int);
		void print_report(int, int, int, int);
};

LCS::LCS() : VERT(1), HORZ(2), DIAG(4) {
	DEL = &LCS::DEL2;
	INS = &LCS::INS2;
	SUB = &LCS::SUB2;
}

void LCS::text1_push_back(string line) {
	text1.push_back(line);	
}

void LCS::text2_push_back(string line) {
	text2.push_back(line);
}

void LCS::compute_alignment() {
	m = text1.size()-1;
	n = text2.size()-1;

	cost.assign(m+1, n+1);
	link.assign(m+1, n+1);

	cost[0][0] = 0;
	link[0][0] = 0;

	for (int i = 1; i <= m; i++) {
		cost[i][0] = cost[i-1][0] + DELcost(text1[i]);
		link[i][0] = VERT;
	}

	for (int j = 1; j <= n; j++) {
		cost[0][j] = cost[0][j-1] + INScost(text2[j]);
		link[0][j] = HORZ;
	}

	for (int i = 1; i <=m; i++) {
		for(int j = 1; j <=n; j++) {
			cost[i][j] = cost[i-1][j-1] + SUBcost(text1[i], text2[j]);
			link[i][j] = DIAG;

			int delcost = cost[i-1][j] + DELcost(text1[i]);
			if (delcost < cost[i][j]) {
				cost[i][j] = delcost;
				link[i][j] = VERT;
			}

			int inscost = cost[i][j-1] + INScost(text2[j]);
			if (inscost < cost[i][j]) {
				cost[i][j] = inscost;
				link[i][j] = HORZ;
			}
		}
	}
}

//public report difference wrapper function
void LCS::report_difference() {
	int i = m, j = n, link_ij = 0;
	report_difference(i, j, link_ij);

	int place = (int)moves.size()-1;
	report_difference(place, 0, 0, 0, 0);
}

//private report difference function that recursibley computes the path
void LCS::report_difference(int i, int j, int link_ij) {
	if ((link_ij=link[i][j]) == 0) {
		return;
	}
	
	if (link_ij == DIAG) {
		i = i-1, j = j-1;
		moves.push_back(DIAG);	
	}
	else if (link_ij == VERT) {
		i = i-1;
		moves.push_back(VERT);
	}
	else {
		j = j-1;
		moves.push_back(HORZ);
	}

	return report_difference(i, j, link_ij);
}

//report difference function pareses the trace and ouputs the results using a separate print funciton
void LCS::report_difference(int place, int ins, int del, int i, int j) {
	if (((place == -1) || (moves[place] == DIAG)) && (ins > 0 || del > 0)) {
		print_report(ins, del, i, j);
		ins = 0;
		del = 0;
	}

	if (place == -1) {
		return;
	}

	if (moves[place] == DIAG) {
		report_difference(--place, ins, del, ++i, ++j);
	}
	else if(moves[place] == VERT) {
		report_difference(--place, ins, ++del, ++i, j);
	}
	else if(moves[place] == HORZ) {
		report_difference(--place, ++ins, del, i, ++j);
	}
}

//print function ouputs changes
void LCS::print_report(int ins, int del, int i, int j) {
	if ((ins > 0 && del == 0) || (ins == 0 && del > 0)) {
		if (ins > 0) {
			cout << i << "a";
			if (ins > 1) {
				cout << (j-ins+1) << "," << j << endl;
			}
			else {
				cout << j << endl;
			}
			for (int k = 0; k < ins; k++) {
				cout << "> " << text2[j-ins+k+1] << endl;
			}
		}
		else {
			if (del > 1) {
				cout << (i-del+1) << "," << i;
			}	
			else {
				cout << i;
			}
			cout << "d";
			cout << j << endl;
			for (int k = 0; k < del; k++) {
				cout << "< " << text1[i-del+k+1] << endl;
			}
		}
	}
	else {
		if (del > 1) {
			cout << (i-del+1) << "," << i;
		}
		else {
			cout << i;
		}
		cout << "c";
		if (ins > 1) {
			cout << (j-ins+1) << "," << j << endl;
		}
		else {
			cout << j << endl;
		}
		
		for (int k = 0; k < del; k++) {
			cout << "< " << text1[i-del+k+1] << endl;
		}
		cout << "---\n";
		for (int k = 0; k < ins; k++) {
			cout << "> " << text2[j-ins+k+1] << endl;
		}
	}
	return;
}

int main(int argc, char *argv[]) {

	if (argc != 3) {
		cerr << "usage: ./Diff8 file1 file2\n";
		return 0;
	}

	LCS lcs;

	string line;

	ifstream fin1;
	fin1.open(argv[1]);
	lcs.text1_push_back("");
	while (getline(fin1, line)) {
		lcs.text1_push_back(line);
		line.clear();
	}

	ifstream fin2;
	fin2.open(argv[2]);
	lcs.text2_push_back("");
	while (getline(fin2, line)) {
		lcs.text2_push_back(line);
		line.clear();
	}

	lcs.compute_alignment();
	lcs.report_difference();

	fin1.close();
	fin2.close();
}
