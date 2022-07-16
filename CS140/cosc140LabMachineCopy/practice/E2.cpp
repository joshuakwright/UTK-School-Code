#include<iostream>

using namespace std;
/*
class list {
	struct node {
		node() { key=0; next=NULL; }
		int key;
		node *next;
	};
	public:
		list() { L = new node; }
//		˜list();
		void magic11A(int);
	//	void magic11B();
		void print();
	private:
		node *L;
};

void list::magic11A(int key) {
	node *p = L, *q = L->next;
	while (q && q->key < key) {
		p = q;
		q = q->next;
	}
	p->next = new node;
	p->next->key = key;
	p->next->next = q;
}
void list::print() {
	node *p = L, *q = L->next;
	for (node *i = L; i->next != NULL; i+=2) {
		cout << p->key << " ";
 		if (q != NULL) {
			p = q;
			q = p->next;
		}
	}
	cout << endl;
}
*/

int magic10(unsigned char number, int k=8) {
	if (number <= 1 || k == 1) {
		return number;
	}
	unsigned char lval = magic10(number >> (k/2), k/2);
	unsigned char rval = magic10(number & ((1<<(k/2)) - 1), k/2);
	return lval + rval;
}

int main() {

	unsigned char num = "3B";
//	cin >> num;
	cout << magic10(num);
	return 0;
	
	//	list L; int key;
//	while (cin >> key)
//		L.magic11A(key);
//	L.print();
//	L.magic11B();
}

