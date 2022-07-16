#include <iostream>

using namespace std;

struct jstruct {
	static int counter;
	static int testo(int& j);
public:
	jstruct();
	~jstruct();
	jstruct(const jstruct& tmp); //copy constructor because it has one parameter that is a constant refrence to an instance
	string sup;
	int id;
//	testy() const; //accesser. testy(); would be a mutator
	testy();
private:
	string hi;
};

int main() {

	jstruct test;
	cout << "test id: " << test.id << " counter " << test.counter << '\n';
	jstruct test2;
	cout << "test2 id: " << test2.id << " counter " << test2.counter << " test counter " << test.counter << '\n';
	int j = 0;
	test.testo(test.id);
	cout << "j: " << j << '\n';

	return 0;
}
int jstruct::counter = 0;

jstruct::jstruct() {
	sup = "hello";
	hi = "yo";
	id = counter++;
}

jstruct::~jstruct() {
	cout << "Josh #:" << id << " has fallen.\n";
}
void jstruct::testy() {
	cout << "Hello my name is: " << this->id << ". I am a Josh. Unfortunately.\n";
}
int jstruct::testo(int& j) {
	j = 420;
	cout << "yo\n";
	return 0;
}
