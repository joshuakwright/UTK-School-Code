//main.cpp
//This lab takes in user input of names and test scores and then outputs an organized list of names and each person's average score. 
//Joshua Wright
//2-Oct-2019
//Professor: Dr.Marz
//Students worked with:
//Dan Ervine

#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>
using namespace std;

int main() {

   vector<double> vectorScore;
   vector<double> vectorCount;
   vector<string> vectorName;
   string name;
   double score;

	cout << "Enter student and grade: ";
	cin >> name;
	cin >> score;

	//while loop keeps taking in input until done is entered
	while (name != "done"){

	//only runs if the input is valid, runs error check if not
	if (!cin.fail()){
		//adds first value to name vector
		vectorName.push_back(name);

		//for loop that will increment score count for each person, add new people, and attack score(s) to each person
		for (unsigned int i = 0; i < vectorName.size(); i++){	
			//this adds new test scores to the same person and increments their total score cound
			if (name == vectorName.at(i) && (i + 1) != vectorName.size()) {
				vectorName.erase(vectorName.begin() + (vectorName.size() - 1));
				vectorCount.at(i) = vectorCount.at(i) + 1.0;
				vectorScore.at(i) = vectorScore.at(i) + score;
			}
			//this adds new names and scores to the vector
			else if ((i + 1) == vectorName.size()) {
				vectorScore.push_back(score);
				vectorCount.push_back(1.0);
			}	

		}
		//allows user to enter new input everytime time while loop loops
		cout << "Enter student and grade: ";
		cin >> name;
		if (name != "done"){
			cin >> score;
		}
	}
	//error checks is the cin for score fails
	else {
		cout << "Re-input grade: ";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');					
		cin >> score;
	}
	}

	//for loop that formats and outputs the students and their score averages
	for (unsigned int i = 0; i < vectorName.size(); i++){
		cout << left << setw(15) << setfill('.') << vectorName.at(i);
		cout << right << setw(10) << setfill('.') << fixed << setprecision(2) << vectorScore.at(i) / vectorCount.at(i);
		cout << "\n";
	}

   return 0;
}
