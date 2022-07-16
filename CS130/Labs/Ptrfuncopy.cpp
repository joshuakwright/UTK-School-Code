#include <fstream>
#include <iostream>
using namespace std;

int strcompare(const char *, const char *);
int strlength(const char *);
void strncopy(char *, const char *, int);

int main(int argc, char *argv[]) {
  check for four command line arguments
  if not, print error message and bail out 

  declare two streams: file1, file2

  declare two char text line arrays 

  if (command line argument is -copy) {
    open file1 for reading
    open file2 for writing

	while (read textline1 from file1) {
	  determine length of textline1
	  copy textline1 to textline2
	  write textline2 + newline to file2 
    }

  } else
  if (command line argument is -compare) {
    open file1 for reading
    open file2 for reading

	for loop keeping track of linenumber {
	  read textline1a from file1
	  read textline2 from file2

	  if (EOF on file1 and file2) 
	    terminate loop

	  if (EOF on file1) {
	    print error message 
	    terminate loop
	  }

	  if (EOF on file2) {
	    print error message 
	    terminate loop
	  }

	  if (textline1 and textline2 are different) {
	    print error message identifying offending lines
	    terminate loop
	  }
    }
  }

  close file1
  close file2
}
