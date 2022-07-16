
class stack {
	struct node {
		node (int data=0);
		int data;
		node *next;
	};

	public:
		stack();
		~stack();

		bool empty() { return N == 0; }
		int size() { return N; }
		void push(const int &din);
		void pop();
		int top();
		int *arr;
	
	private:
		int N;
		node *head;
		node *findnode(int);
};

stack::node::node(int n_data) {
	data = n_data;
	next = NULL;
}

stack::stack() {
	arr = new int(10);
	head = arr;
	N = 0;
}

stack::~stack() {
	clear();
	delete head;
	delete arr;
}

void stack::push(const int &din) {
	int *i = head + N;
	*i = din;
	N++;
}

void stack::pop() {
	int *i = head + N;
	*i = 0;
	N--;
}

int stack::top() {
	int *i = head + N;
	int j = *i;
	return j;
}

// Problem 2: Error checking should check that you are not pushing more than 10 values 
