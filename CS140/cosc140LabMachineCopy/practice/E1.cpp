#include<iostream>

using namespace std;

int main() {

	char buf[6] = {0};
	char *s1;
	s1 = buf;

	const char *s2 = "hello";

	char *s = s1;
	*s = *s2;
	do {
		s++;
		s2++;
		*s = *s2;
	} while (*s2 != '\0');


	while (*s1 != '\0') {
		cout << *s1;
		s1++;
	}

	return 0;

}
