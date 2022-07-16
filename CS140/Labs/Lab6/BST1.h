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
/*
  public:
    class iterator {
      public:
       default constructor (no argument)
       overloaded operators (++, *, ==, !=)
      private:
        friend class bst<TKey>;
        constructor (with argument)

        node *p;
    };

    iterator begin() { ... }
    iterator end() { ... }
*/
  public:
    bst() { Troot=NULL; ID = 0; }
    ~bst() { clear(Troot); }

    bool empty() { return Troot==NULL; }

    void insert(TKey &);

//    iterator lower_bound(const TKey &);
//    iterator upper_bound(const TKey &);

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
//  change below to output subtree ID information

	if (left)  cout << " L=" << setw(3) << left->key;
	else       cout << "      ";
	if (right) cout << " R=" << setw(3) << right->key;
	else       cout << "      ";

	cout << "\n";
}

//bst<TKey>::iterator functions not defined above go here

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

//bst<TKey>::lower_bound function goes here

//bst<TKey>::upper_bound function goes here

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
