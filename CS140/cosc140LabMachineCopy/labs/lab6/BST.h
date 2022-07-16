
//Joshua Wright 11-10-20 Lab 6 Greggor
//This is a basic implementation of the BST header file
#ifndef BST_H
#define BST_H

#include <iomanip>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

template <class TKey>
class bst {
  struct node {
    node(int id = 0);

    void print();

    TKey key;

    node *left;
    node *right;
	node *parent;
	int ID;
  };

  public:
   //iterator subclass defining iterators and operator overloads
	class iterator {
      public:
       iterator() {p = NULL;}
	   TKey & operator*() {return p->key;}
	   iterator & operator++();
	   bool operator==(const iterator &rhs) const {return p == rhs.p;}
	   bool operator!=(const iterator &rhs) const {return !operator==(rhs);}
      private:
        friend class bst<TKey>;
        iterator(node *np) {p = np;}

        node *p;
    };

    iterator begin();
    iterator end() {return iterator(NULL);}

  public:
    bst() { Troot=NULL; ID = 0; }
    ~bst() { clear(Troot); }

    bool empty() { return Troot==NULL; }

    void insert(TKey &);

    iterator lower_bound(const TKey &);
    iterator upper_bound(const TKey &);

    void print_bylevel();

  private:
    void clear(node *);
    node *insert(node *, TKey &);

    int ID;
    node *Troot;
};

template <class TKey>
bst<TKey>::node::node(int id) {
	ID = id;
	parent = NULL;
}

template <class TKey>
void bst<TKey>::node::print()
{
  cout << std::right << setw(3) << ID;
  cout << setw(4) << key << " :";
  if (parent) {
	cout << " P=";
	cout << std::right << setw(3) << parent->ID;
  }
  else {
	cout << " ROOT ";
  }

  if (left)  cout << " L=" << setw(3) << left->ID;
  else       cout << "      ";
  if (right) cout << " R=" << setw(3) << right->ID;
  else       cout << "      ";

  cout << "\n";
}

template <>
void bst<string>::node::print() {
	cout << right << setw(3) << ID;
	cout << setw(20) << key << " :";
	if (parent) {
		cout << " P=";
		cout << right << setw(3) << parent->key;
	}
	else {
		cout << " ROOT ";
	}

	if (left)  cout << " L=" << setw(3) << left->key;
	else       cout << "      ";
	if (right) cout << " R=" << setw(3) << right->key;
	else       cout << "      ";

	cout << "\n";
}

//pre increment operator overload for iterator subclass that increments throught the BST
//in order from least to greatest
template <class TKey>
typename bst<TKey>::iterator & bst<TKey>::iterator::operator++() {
	node *l = p;

	if (p->right != NULL) {
		p = p->right;
		while (p->left != NULL) {
			p = p->left;
		}
	}
	else {
		p = p->parent;
		if (!p) {
			p = NULL;
			return *this;
		}
		while (l == p->right) {	
			l = p;
			p = p->parent;
			if (!p) {
				p = NULL;
				return *this;
			}
		}
	}

	return *this;
}

//returns an iterator to the first element in the BST
template <class TKey>
typename bst<TKey>::iterator bst<TKey>::begin() {
	if (Troot == NULL) {
		return iterator(NULL);
	}
	node *temp = Troot;
	while (temp->left != NULL) {
		temp = temp->left;
	}
	return iterator(temp);
}

template <class TKey>
void bst<TKey>::clear(node *T)
{
  if (T) {
    clear(T->left);
    clear(T->right);
    delete T;
    T = NULL;
  }
}

template <class TKey>
void bst<TKey>::insert(TKey &key)
{ 
	Troot = insert(Troot, key);
}

//inserts key values into the BST by incrementing to placement by the key value
//and then creating a new node in that place, storing the new key
template <class TKey>
class bst<TKey>::node *bst<TKey>::insert(node *T, TKey &key)
{
  if (T == NULL) {
	T = new node(++ID);
    T->key = key;
  } 
  else if (T->key == key) {
    ;
  } 
  else if (key < T->key) {
	T->left = insert(T->left, key);
	T->left->parent = T;
  } 
  else {
    T->right = insert(T->right, key);
	T->right->parent = T;
  }
  return T;
}

//returns an iterator pointing to the node with the key value <= that of the one inserted
template <class TKey>
typename bst<TKey>::iterator bst<TKey>::lower_bound(const TKey &key) {
	node *t = Troot;
	node *n = NULL;
	while (t != NULL) {
		if (key <= t->key) {
			n = t;
			t = t->left;
		}
		else {
			t = t->right;
		}
	}
	return iterator(n);
}

//returns an iterator pointing to the node with the key value < that of the one inserted
template <class TKey>
typename bst<TKey>::iterator bst<TKey>::upper_bound(const TKey &key) {
	node *t = Troot;
	node *n = NULL;
	while (t != NULL) {
		if (key < t->key) {
			n = t;
			t = t->left;
		}
		else {
			t = t->right;
		}
	}
	return iterator(n);
}

template <class TKey>
void bst<TKey>::print_bylevel()
{
  if (Troot == NULL)
    return;

  queue<node *> Q;
  node *T;

  Q.push(Troot);
  while (!Q.empty()) {
    T = Q.front();
    Q.pop();

    T->print();
    if (T->left)  Q.push(T->left);
    if (T->right) Q.push(T->right);
  }
}
#endif
