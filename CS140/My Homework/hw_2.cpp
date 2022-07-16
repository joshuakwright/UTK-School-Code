#include<iostream>
#include<iomanip>

using namespace std;

class list {

	public:
		list(int Nmax = 0);
		void push_front(const int &);
		void pop_front();
		const int & front();
		int size() const { return N;}
		int & operator[] (int);

	private: 
		int N, Nmax;
		int *data;
};

list::list(int n_N) {
	N = 0;
	Nmax = n_N;

	data = new int[Nmax];
	for (int i = 0; i < Nmax; i++) {
		data[i] = 0;
	}
}

void list::push_front(const int &din) {
	int M = Nmax + 1;  
	int *newdata = new int[M];

	for (int i = 0; i<Nmax; i++) {  
		newdata[i + 1] = data[i];
	}
	
	newdata[0] = 0;  

	delete [] data;  
	data = newdata;  
		
	Nmax = M;

	data[0] = din;  
	
	N++;
}

void list::pop_front() {
  
	data[0] = 0;

}

const int & list::front() {

	return data[0];

}

int & list::operator[](int i) { return data[i]; }

void printlist(const char *operation, list &v) {  
	cout << setw(14) << operation << " s=" << v.size() << " : ";  
	for (int i=0; i<v.size(); i++){   
		cout << " " << v[i];  cout << "\n";}
}

int main() {

	list v;		printlist("list v", v);

	v.push_front(7);	printlist("v.push_front(7)", v);
	v.push_front(8);	printlist("v.push_front(8)", v);
	v.push_front(9);    printlist("v.push_front(9)", v);
	v.pop_front();		printlist("v.push_front()", v);


	return 0;
}
