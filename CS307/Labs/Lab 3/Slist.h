#ifndef SLIST_H
#define SLIST_H

#include <algorithm>
#include <vector>

//quicksort algorithm implemented using a randomly chose pivot and a 3-way partition {<}, {=}, {>}
template <typename T>
void qsort_r3(vector<T> &AP, int left, int right) {

	if (right <= left) {
		return;
	}

	int pindex;
	int randpart = rand() % (right - left + 1) + left;

	//select pivot
	pindex = randpart;
	T pivot = AP[pindex]; 
	//partition A: {<}, {=pivot}, {>}   
	swap(AP[pindex], AP[right]);  
	int i = left - 1;  
	int j = right;   
	int k = left;
	int l = right - 1;

	while (1) {    
		while (AP[++i] < pivot) { } 
		while (pivot < AP[--j] && j > i) { }    
		if (i>=j) break;  
		swap(AP[i], AP[j]);
		//puts = pivot values at ends
		if (AP[i] == pivot) {
			swap(AP[k], AP[i]);
			k++;
		}
		if (AP[j] == pivot) {
			swap(AP[l], AP[j]);
			l--;
		}
	}  

	pindex = i;  
	swap(AP[pindex], AP[right]);  

	//puts = pivot values next to pivot
	i = pindex - 1;
	j = pindex + 1;
	for (int r = left; r < k - 1; r++, i--) swap(AP[r], AP[i]); 
	for (int r = right - 1; l + 1 < r; r--, j++) swap(AP[j], AP[r]); 

	qsort_r3(AP, left, i);
	qsort_r3(AP, j, right);
}

template <class T>
class slist {
	private:
		struct node {
			node() { data = T(); next = NULL; }
			node(const T &key) { 
				data = key;
				next = NULL;
			}

			bool operator<(const T &rhs) const { return data < rhs; }

			T data;
			node *next;
		};

		//smart pointer points to node data for linked list sorting using an array
		class sptr {
			public:    
				sptr(node *_ptr=NULL) { ptr = _ptr; }    

				bool operator< (const sptr &rhs) const {      
					return ptr->data < rhs.ptr->data;    
				}

				operator node * () const { return ptr; }  
			private:    
				node *ptr;
		};

	public:
		class iterator {
			public:
				iterator() { p = NULL; }
				T & operator*() { return p->data; }
				iterator & operator++() { p = p->next; return *this; }
				bool operator!=(const iterator & rhs) const { return p != rhs.p; }

			private:
				iterator(node *n_p) { p = n_p; }
				node *p;

				friend class slist<T>;
		};

	public:
		slist();
		~slist();

		void push_back(const T &);
		void sort(const string &);

		iterator begin() { return iterator(head->next); }
		iterator end() { return iterator(NULL); }

	private:
		node *head;
		node *tail;
};

template <typename T>
slist<T>::slist() {
	head = new node();
	tail = head;
}

template <typename T>
slist<T>::~slist() {
	while (head->next != NULL) {
		node *p = head->next;
		head->next = p->next;
		delete p;
	}
	delete head;

	head = NULL;
	tail = NULL;
}

template <typename T>
void slist<T>::push_back(const T &din) {
	tail->next = new node(din);
	tail = tail->next;
}

//sorts linked list using a smart pointer and stl::sort, stl::stable_sort, or custom quicksort function
template <typename T>
void slist<T>::sort(const string &algname) {

	node *temp = head;
	int Asize = 0;
	while (temp != tail) {
		temp = temp->next;
		Asize++;
	}

	//creates vector of smart pointers from linked list for sorting
	vector<sptr> Ap(Asize);
	temp = head->next;
	for (int i = 0; i < Asize; i++) {
		sptr isp(temp);
		Ap[i] = isp;
		temp = temp->next;
	}

	if (algname == "-quicksort") {
		std::sort(Ap.begin(), Ap.end());
	}
	else if (algname == "-mergesort") {
		std::stable_sort(Ap.begin(), Ap.end());
	}
	else if (algname == "-qsort_r3") {
		qsort_r3(Ap, 0, Ap.size() - 1);
	}

	//relinks linked list based on sorted array
	temp = head;
	for (int i = 0; i < Asize; i++) {
		sptr tsptr = Ap[i];
		temp->next = tsptr;
		temp = temp->next;
	}
	temp->next = NULL;

}
#endif // SLIST_H
