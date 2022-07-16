#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

typedef unsigned int uint;

typedef union {
  uint  binary;
  float ieee754;
} fpnum;

// #define SIGN  ((uint)0x80000000)
// #define BEXP  ((uint)0x7f800000)
// #define FRAC  ((uint)0x007fffff)
// #define BIT23 ((uint)0x00800000)

void unpack(fpnum N, uint &s, int &e, uint &f) {
	s = (N.binary & 0x80000000) >> 31;
	e = (N.binary & 0x7f800000) >> 23;
	f = N.binary & 0x007fffff;
	if ((e != 0x00000000) && (f != 0x00000000)) {
		e -= 127;
		f += 0x00800000;
	} 
}

void package(fpnum &R, uint s, int e, uint f)
{
	if (f != 0x00000000) {
		e += 127;
		f -= 0x00800000;
	}
 
	R.binary = ((s << 31) | (e << 23) | f);
}

void align_exponents(uint &f1, uint &f2, int &e1, int &e2) {
  if (e1 > e2) {
    if ((e1 - e2) > 31) {
      f2 = 0x00000000;
    }
    else {
      int shift = e1 - e2;
      e2 += shift;
      f2 = f2 >> shift;
    }
  }
  else if (e1 < e2) {
    if ((e2 - e1) > 31) {
      f1 = 0x00000000;
    }
    else {
      int shift = e2 - e1;
      e1 += shift;
      f1 = f1 >> shift;
    }
  }
}

void align_leftmost_onebit(uint &f, int &e) {
  uint fc = f;
  if (fc != 0x00000000) {
    if ((fc & 0x8f800000) == 0x00000000) {
      int shift = 0;
      for (int i = 0; i < 23; i++) {
        if ((fc & 0x00800000) == 0x00800000) {
          f = fc;
          e = e - shift;
          break;
        }
        else if ((fc &0x00800000) == 0x00000000) {
          fc = fc << 1;
          shift++;
        }
      }
    }
  
    else {
      int shift = 8;
      for (int i = 0; i < 9; i++) {
        if ((fc & 0x80000000) == 0x0000000) {
         if ((fc & 0x80000000) == 0x8000000) {
           break;
         }
          shift--;
          fc = fc << 1;
        }
    }
    f = f >> shift;
    e = e + shift;
    }
 } 
}

//This is the only function I couldn't get to totally work. It works up to test case 126 for the 
//test script, which I know is where is begins with subtraction. But despite my best efforts, I 
//couln't figure out what's wrong. Logically I feel like everything is sound, so I presume it must
//be some small error in syntax or numbers that I'm missing. 
void floatcal(fpnum &S, fpnum X, fpnum Y, string op) {
  uint Xs;
  int Xe;
  uint Xf;
  uint Ys;
  int Ye;
  uint Yf;
  uint Ss;
  int Se;
  uint Sf;
  //unpacks and aligns
  unpack(X, Xs, Xe, Xf);
  unpack(Y, Ys, Ye, Yf);
  align_exponents(Xf, Yf, Xe, Ye);
  Se = Xe;
  
  //takes 2's complement if negative
  if (Xs == 0x00000001) {
    Xf = (Xf ^ 0x007fffff) + 0x00000001;
  }
  if (Ys == 0x00000001) {
    Yf = (Yf ^ 0x007fffff) + 0x00000001;
  }
  
  //addition
  if (op == "+") {
    Sf = Xf + Yf;
  }
  //subtraction
  else if (op == "-") {
    Yf = (Yf ^ 0x007fffff) + 0x00000001;
    Sf = Xf + Yf;
  }

  //assinment of sign value
  if ((Sf & 0x80000000) == 0x80000000) {
    Ss = 0x00000001;
  }
  else if ((Sf & 0x80000000) == 0x00000000) {
    Ss = 0x00000000;
  }
 
  //takes 2's complement if final value is negative
  if (Ss == 0x00000001) {
    Sf = (Sf ^ 0x007fffff) + 0x00000001;
  }
  
  //aligns bits and packages
  align_leftmost_onebit(Sf, Se);
  package(S, Ss, Se, Sf);
}

string hexstring(int K, uint N) {
  string output;
  string hex[16] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f"};
  uint Np = N;
  for (int i = 0; i < K; i++) {
    int place = Np & 15;
    Np = Np >> 4;
    output.insert(0, hex[place]);
  }
  return "0x" +output;
}

string fpnum2str(fpnum N) {
  ostringstream ss;
  uint s;
  int e;
  uint f;
  string mid;
  string output;
  string signchar;
  unpack(N, s, e, f);
  
  ss << fixed << scientific << setprecision(6) << N.ieee754;
  string floatnum = ss.str();
  ss.str("");
  
  string sign = hexstring(1, s);
  if (sign == "0x0") {
    signchar = "+";
  }
  else if (sign == "0x1"){
    signchar = "-";
  }
  if (sign == "0x0") {
    sign = "0";
  }
  else if (sign == "0x1") {
    sign = "1";
  }
  
  string frac = hexstring(6, f);
  e += 127;
  string exp = hexstring(2, e);
  int ep = e - 127;
  string hexflo = hexstring(8, N.binary);
  
  fpnum ft;
  uint ut = 0;
  int it = 0;
  package(ft, ut, it, f);
  ss << fixed << ft.ieee754;
  string last = ss.str();
  ss.str("");
  
  string starter = " :  ";
  if (signchar == "-") {
      starter = " : ";
  }
   
  ss << " : " << hexflo << " : " << sign << " : " << exp << " : " << frac
  << " : " << signchar << last << " 2^" << ep;
  mid = ss.str();
  ss.str("");
  
  ss << starter << floatnum << mid;
  output = ss.str();
  ss.str("");
   
  return output;
}

int main(int argc, char *argv[]) {
    fpnum X;
    fpnum Y;
    string op;

	while (1) {
      cout << "X>  ";
      cin >> X.ieee754;
      if (cin.eof())
        return 1;

      cout << "Y>  ";
      cin >> Y.ieee754;
      if (cin.eof())
        return 1;

      cout << "op> ";
      cin >> op;
      if (cin.eof())
        return 1;

      // Done reading input, now execute

	  fpnum R;	// machine result
	  fpnum S;	// program result
  
	  if (op.compare("+") == 0)
	    R.ieee754 = X.ieee754 + Y.ieee754;
	  else
	  if (op.compare("-") == 0)
	    R.ieee754 = X.ieee754 - Y.ieee754;
	  else {
        cout << "error: unknown operation\n\n";
        continue;
      }

	  floatcal(S, X, Y, op);

	  cout << "X" << fpnum2str(X) << "\n"
	       << "Y" << fpnum2str(Y) << "\n"
		   << "S" << fpnum2str(S) << "\n"
		   << "R" << fpnum2str(R) << "\n\n";
    }
}
