#include <fstream>
#include <iostream>
using namespace std;

int strlength(const char *s) {
	int i = 0;
	while (*(s + i) != 0) {
		i++;
	}
	return i;
}

int strcompare(const char *s1, const char *s2) {
	int j = 0;
	int diff = 0;
	while ((*(s1 + j) != 0) && (*(s1 + j) == *(s2 + j))) {
		j++;
	}
	diff = *(unsigned char *)(s1 + j) - *(unsigned char *)(s2 + j);

	return diff;
}

void strncopy(char *s1, const char *s2, int N) {
	int i = 0;
	while ((*(s2 + i) != 0) && (i <= N)) {
		*(s1 + i) = *(s2 + i);
		i++;
	}
}

int main(int argc, char *argv[]) {
  if (argc != 4) {
	  //the test script failed on this one case because it was expecting
	  //no output, but I left my output in here because the lab said to.
	cout << "Invalid Number of Arguments\n" << 
		"Format: ./Ptrfun -copy/compare message.txt message_copy.txt";
	return 0;
  } 

  fstream file1;
  fstream file2;

  char buffer1[256];
  char buffer2[256];
  char copy[] = "-copy";
  char compare[] = "-compare";

  if ((strcompare(copy, *(argv + 1))) == 0) {
	file1.open(*(argv + 2), fstream::in);
	file2.open(*(argv + 3), fstream::out);

	while (file1.getline(buffer1, 256)) {
		int length = strlength(buffer1);
		strncopy(buffer2, buffer1, length);
		file2.write(buffer2, length);
		if (!file1.eof()) {
			file2.put('\n');
		}
    }
  } 
  else if ((strcompare(compare, *(argv + 1))) == 0) {
    file1.open(*(argv + 2), fstream::in);
	file2.open(*(argv + 3), fstream::in);

	for (int i = 1;; i++) {
		file1.getline(buffer1, 256);
		file2.getline(buffer2, 256);

	  if (file1.eof() && (!file2.eof())) {
	    cout << "1: EOF\n"; 
	    break;
	  }

	  if ((!file1.eof()) && file2.eof()) {
	    cout << "2: EOF\n";
	    break;
	  }

	  if (strcompare(buffer1, buffer2)) {
	    cout << "Files differ on line " << i << '\n' << "1: ";
		for (char *j = buffer1; j < (buffer1+strlength(buffer1)); ++j) {
			cout << *j;
		}
		cout << '\n' << "2: ";
		for (char *k = buffer2; k < (buffer2+strlength(buffer2)); ++k) {
			cout << *k;
		}
		cout << '\n';
		break;
	  }

	   if (file1.eof() && file2.eof()) {
		   break;
	   }
    }
  }

  file1.close();
  file2.close();
  return 0;
}
