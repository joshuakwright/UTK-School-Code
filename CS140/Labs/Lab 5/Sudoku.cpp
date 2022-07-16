
//Joshua Wright Lab 5 Greggor 10/27/2020
//This program takes in sets of three numbers in the the for of indices of a 9x9 grid followed 
//by a number 1-9 to (making a partial sudoku board) and outputs the solution of the game and
//the time it took to complete
#include <sys/time.h>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
using namespace std;

//global units index
const char *UNITS[] {"sec", "msec", "usec"};

class omega {
  public:
	omega();
	float get_sec();

  private:
    long Tinit;
    long Tmark;
};

omega::omega() {
  struct timeval T;
  gettimeofday(&T, NULL);

  Tinit = 1000000*T.tv_sec + T.tv_usec;
}

float omega::get_sec() {
  struct timeval T;
  gettimeofday(&T, NULL);

  Tmark = 1000000*T.tv_sec + T.tv_usec;

  return (float)(Tmark-Tinit)/1000000.0;
}

//uses recursion to convert the time elapsed in seconds to the most favorable unit for that interval
string elapsed(float duration, int i = 0) {
  if (duration < 0.1) {
	return elapsed(duration*1000, i+1);
  }
  else {
	char buffer [10];
	sprintf(buffer, "%0.2f %s", duration, UNITS[i]);
	return buffer;
  }
}

class sudoku {
  public:
	sudoku();

	void solve();

    void read(const char *);
    void write(const char *);
    void write(const char *, const char *);

  private:
    bool solve(vector<int>, int);

    // various support functions 
    bool error_check_value(bool);
    bool error_check_uniqueness();
	bool check_row(int, int);
	bool check_col(int, int);
	bool check_square(int, int, int);
	vector<int> valid_values(int i, int j);

    void display();

    int game[9][9];
};

sudoku::sudoku() { 
  for (int i=0; i<9; i++) {
    for (int j=0; j<9; j++)
      game[i][j] = 0;
  }
}

//sudoku solving initilaizer that pushes all the open grid slots to the solving algorithm and then error checks the output
void sudoku::solve() {
  cout << "SOLVE\n";

  vector<int> cells;
  int temp;

  for (int i = 0; i < 9; i++) {
	for (int j = 0; j < 9; j++) {
		if (game[i][j] == 0) {
			//indices passed in 2-1 format so as to use less space
			temp = (i * 9) + j;
			cells.push_back(temp);
		}
	}
  }

  if (solve(cells, 0)) {display();}
  if (error_check_value(false) || error_check_uniqueness()) {exit(0);}
}

//reads in initail table values from file and error checks the input
void sudoku::read(const char *fname) {
  cout << "READ\n";
  bool err = false;
  int line = 1;

  ifstream fin(fname);
    
  int i, j, v;

  while (fin >> i >> j >> v) {
	if (i < 0 || i > 8 || j < 0 || j > 8) { 
		err = true;
		cerr << "line " << line << ": " << i << " " << j << " " << v << " out-of-bounds grid index\n";
	}
    game[i][j] = v;
	line++;
  }
  fin.close();

  if (err) {exit(0);}
  
  if (error_check_value(true) || error_check_uniqueness()) {
	  display();
	  exit(0);
	}
  display();
}

void sudoku::write(const char *fname) {
  ofstream fout(fname);

  for (int i=0; i<9; i++) {
    for (int j=0; j<9; j++) {
      if (0 < game[i][j]) {
        fout << i << " "
			 << j << " "
		     << game[i][j] << "\n";
      }
    }
  }

  fout.close();
}

void sudoku::write(const char *fname, const char *addon) {
  int N1 = strlen(fname);
  int N2 = strlen(addon);

  char *n_fname = new char[N1+N2+2];

  // strip .txt suffix, then concatenate _addon.txt
  strncpy(n_fname, fname, N1-4);
  strcpy(n_fname+N1-4, "_");
  strcpy(n_fname+N1-3, addon);
  strcpy(n_fname+N1-3+N2, ".txt");

  write(n_fname);

  delete [] n_fname;
}

//uses recursion to progress through the partial sudoku table, backtracking when all possibilities have 
//been exhausted for one path until the table is completed
bool sudoku::solve(vector<int> cells, int c) { 
	if (c == cells.size()) {return true;}
	
	int i = cells[c] / 9;
	int j = cells[c] % 9;

	vector<int> values = valid_values(i, j);

//finds the open grid space with the lease possible valid values and switches the current grid space
//with that one to optomize the solve time. 
	vector<int> lowest_vv = values;
	int lowest_vv_i = c;
	int min = values.size();
	for (int l = c + 1; l < cells.size(); l++) {
		int m = cells[l] / 9;
		int n = cells[l] % 9;
		vector<int> temp = valid_values(m, n);
		if (temp.size() < min) {
			lowest_vv_i = l;
			min = lowest_vv.size();
			lowest_vv = temp;
		}
	}
	swap(cells[c], cells[lowest_vv_i]);

	i = cells[c] / 9;
	j = cells[c] % 9;

	values = valid_values(i, j);

	if (values.size() == 0) {
		return false;
	}

	for (int k = 0; k < values.size(); k++) {
		game[i][j] = values.at(k);
		if (solve(cells, c+1) == true) {
			return true;
		}
	}

	game[i][j] = 0;
	return false;
}

//checks to make sure all input table values are within the correct range (0-9 before solution and 1-9 after solved)
bool sudoku::error_check_value(bool zero_valid) {
	bool err = false;

	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (zero_valid) {
				if ((game[i][j] < 0) || (game[i][j] > 9)) {
					err = true;
					cerr << "cell " << i << " " << j << ": out-of-bounds data value " << game[i][j] << "\n";
				}
			}
			else {
				if ((game[i][j] < 1) || (game[i][j] > 9)) {
					err = true;
					cerr << "cell " << i << " " << j << ": out-of-bounds data value " << game[i][j] << "\n";

				}
			}
		}
	}
	return err;
}

//checks to make sure an input number follows the sudoko rules with only one of the 
//same number in each row, column, and 3x3 square
bool sudoku::error_check_uniqueness() {
	int val;
	bool err = false;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			val = game[i][j];
			if (val != 0) {
				if (check_row(i, val)) {
					err = true;
					cerr << "cell " << i << " " << j << ": non-unique value " << val << "\n";
				}
				if (check_col(j, val)) {
					err = true;
					cerr << "cell " << i << " " << j << ": non-unique value " << val << "\n";
				}
				if (check_square(i, j, val)) {
					err = true;
					cerr << "cell " << i << " " << j << ": non-unique value " << val << "\n";
				}
			}
		}
	}
	return err;
}

//checks row for uniqueness
bool sudoku::check_row(int r, int v) {
	bool err = false;
	int incr = 0;
	for (int i = 0; i < 9; i++) {
		if (game[r][i] == v) {incr++;}
	}
	if (incr > 1) {err = true;}
	return err;
}

//checks column for uniqueness
bool sudoku::check_col(int c, int v) {
	bool err = false;
	int incr = 0;
	for (int i = 0; i < 9; i++) {
		if (game[i][c] == v) {incr++;}
	}
	if (incr > 1) {err = true;}
	return err;
}

//checks 3x3 square for uniqueness
bool sudoku::check_square(int i, int j, int v) {
	int incr = 0;
	bool err = false;
	int si = (i / 3) * 3;
	int sj = (j / 3) * 3;

	for (int k = si; k < (si + 3); k++) {
		for (int l = sj; l < (sj + 3); l++) {
			if (game[k][l] == v) {incr++;}	
		}
	}

	if (incr > 1) {err = true;}
	return err;
}

//returns all the valid numbers for a given space
vector<int> sudoku::valid_values(int i, int j) {
	int temp = game[i][j];
	vector<int> validvals;
	bool err = true;

	for (int k = 1; k <= 9; k++) {
		game[i][j] = k;
		if (check_row(i, k) || check_col(j, k) || check_square(i, j, k)) {err = false;}
		if (err) { validvals.push_back(k);}
		err = true;
	}
	game[i][j] = temp;
	return validvals;
}

void sudoku::display() {
  cout << "| --------------------------- |\n";
  for (int i=0; i<9; i++) {
    for (int j=0; j<9; j++) {
	  if (j == 0) 
	    cout << "| " << game[i][j];
	  else if (j%3 == 0) 
	    cout << " | " << game[i][j];
	  else
	    cout << "  " << game[i][j];
	}
    cout << " |\n";
	if (i%3 == 2)
      cout << "| --------------------------- |\n";
  }
}

int main(int argc, char *argv[]) {
  srand(time(NULL));

  if ((argc != 3) ||
      (strcmp(argv[1], "-s") != 0) ||
	  strstr(argv[argc-1], ".txt") == NULL) {
    cerr << "usage: Sudoku -s game.txt\n";
	exit(0);
  }

  omega timer;
  sudoku sudoku_game;

  float T0 = 0.0;
  float T1 = 0.0;

  if (strcmp(argv[1], "-s") == 0) {
    sudoku_game.read(argv[2]);

	T0 = timer.get_sec();
    sudoku_game.solve();
	T1 = timer.get_sec();

    sudoku_game.write(argv[2], "solved");
  }

  string elapsed_time = elapsed(T1-T0);
  cout << "Time: " << elapsed_time << " (" << setprecision(6) << (T1-T0) << " sec)\n";
}
