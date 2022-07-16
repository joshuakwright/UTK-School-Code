#include <iostream>
using namespace std;	
int main() {
    int left; // creates left integer
    int right; //creates right integet
    cout << "Enter left and right operands: "; //outputs that sentence
    cin >> left >> right; //allows user to input left and right variable
    cout << left << " + " << right << " = " << (left + right) << '\n'; //outputs left +right
    return 0;
}
