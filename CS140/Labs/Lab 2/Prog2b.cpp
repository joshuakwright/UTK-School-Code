#include<sstream>
#include<iomanip>
#include <cstdlib>
#include <iostream>

using namespace std;

const string face[] = { "Ace", "2", "3", "4", "5", "6", "7",
	                        "8", "9", "10", "Jack", "Queen", "King" };
const string suit[] = { "Clubs", "Diamonds", "Hearts", "Spades" };

//list class
class list {
	struct node {
		node (int data = 0); 
		int data;
		node *next;
	};
	
	public: 
		list();
		~list();

		bool insert(int);

	private:
		node *head;

	friend ostream & operator<<(ostream &, const list &);
};

//node constructor
list::node::node(int n_data) {
	data = n_data;
	next = NULL;
}

//list constructor
list::list() {
	head = new node;
}

//list deconstructor
list::~list() {

	delete head;

}

//list insertion funciton
bool list::insert(int rank) {
	node *pp = new node(rank); 
	node *prev, *p;
	prev = head;
	p = prev->next;
	bool temp = false;

	while (p != NULL) {
		if (p->data > rank) {
			pp->next = prev->next;
			prev->next = pp;
			temp = true;
			break;
		}
		else if(p->data == rank) {
			prev->next = p->next;
			delete p;
			break;
		}
		prev = p;
		p = prev->next;

	}

	if (p == NULL) {
		pp->next = prev->next;
		prev->next = pp;
		return true;
	}

	return temp;
}

//ostream << opersator overload function
ostream & operator<<(ostream & out, const list & G) {

	list::node *p;
	p = G.head->next;
	int i = 0;

	while (p != NULL) {
		out << face[p->data] << " ";
		 if ((p->data == 10) || (p->data == 11) || (p->data == 12)) {
             i++;
         }
		 p = p->next;
	}

	if (i == 3) {
		out << " **";
	}

	out << "\n";
	
	return out;
}

//creates a string with a random card
string random_card(bool verbose=false) {
	string card;

	card = face[ rand()%13 ];
	card += " of ";
	card += suit[ rand()%4 ];

	if (verbose)
	  cout << card << "\n";

	return card;
}

//parses a card string into the suit and face
void decode_card(const string &name, int &f, int &s) {
	//put suit and face in its own string
	string a, b, c;
	istringstream iss;
	iss.str(name);

	iss >> a >> b >> c;

	//performs a string comparison to find the array index of the suit and then face of the card
	for (int i = 0; i < 13; i++) {
		if (a == face[i]) {
			s = i;
		}
	}

	for (int j = 0; j < 4; j++) {
		if (c == suit[j]) {
			f = j;
		}
	}
}

int main(int argc, char *argv[])
{
	bool verbose = false;
	int seedvalue = 0;

	for (int i=1; i<argc; i++) {
	  string option = argv[i];
	  if (option.compare(0,6,"-seed=") == 0) {
	    seedvalue = atoi(&argv[i][6]);
	  } else if (option.compare("-verbose") == 0) {
	    verbose = true;
	  } else 
	    cout << "option " << argv[i] << " ignored\n";
	}

	srand(seedvalue);

	int tablec[4][13] = {0};
	list table[4];
	
	//loop that creates table of cards and then stops when one of each face card is found for one suit
	int k;
	while (1) {
	  string card = random_card(verbose);
	  int f, s;

		decode_card(card, f, s);

		//fills array of lists
		if (table[f].insert(s)) {
			tablec[f][s]++;
		}
		else {
			tablec[f][s]--;
		}

		//loop through each face and sees if there are at least one of each face card. If so, the while loop is broken and the face that broke it is recorded
		int m = 0;
		bool stop = false;
		for (k = 0; k < 4; k++) {
			for (int l = 10; l < 13; l++) {
				if (tablec[k][l] != 0) {
					m++;
				}
			}	
			if (m == 3) {
				stop = true;
			}
			else {
				m = 0;
			}
			if (stop) {
				break;
			}
		}
		if (stop) {
			break;
		}
	}

	//outputs table of cards and flags the one that broke the loop with "**"
	for (int i = 0; i < 4; i++) {
		cout << right << setw(8) << suit[i];
		cout << " : ";
		cout << table[i];
	}	
}


