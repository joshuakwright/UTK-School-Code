#include <iostream>
/*this function will update each element of "array" by adding the percentage back into the array.
 * Finally, this function will return the average of all elements of the array after the percentages
 * have been added.*/
using namespace std;
double update(int array[], int size, double percentage);

int main() {

	int array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	int size = 9;
	double percentage = 0.5;
	double average = update(array, size, percentage);
	cout << average;

	return 0;
}

double update(int array[], int size, double percentage) {
	double total;
	for (int i = 0; i < size; i++) {
		array[i] = array[i] + (array[i] * percentage);
		total = total + array[i];
	}
	double average = total / size;
	return average;

}

