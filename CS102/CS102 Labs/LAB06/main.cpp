//main.cpp
//This lab reads in names and scores from an external text file and then creates a formatted and organized list of the requested amount of names and corresponding total scores.
//Joshua Wright
//9-Oct-2019
//Professor: Dr.Marz
//Students worked with:
//Dan Ervine
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>
using namespace std;

int main(int argc, char*argv[]) {

	//error checks for invalid amount of command line input values
	if (argc < 3) {
		cout << "Usage: ./a.out <filename> <names to print>\n";
	} 

	//runs funtion if valid input
	else {

		//opens file
		ifstream fin;
		fin.open(argv[1]);

		//variables and vectors
		vector<double> vectorScore;
		vector<string> vectorName;
		string name;
		double score;
		string line;
		unsigned int nameCount;

		//takes in requested amount of names and outputs a fail message if namecount input was invalid
		istringstream sin(argv[2]);
		sin >> nameCount;
		if (sin.fail()) {
			cout << "Unable to convert names to print.\n";
			return -1;
		}
		sin.clear();

		//loop that populates the vectors for score and names while eliminating duplicates and adding scores of same person, loops unitl all lines have been read
		while (getline(fin, line)) {

			//takes in line string and separates into string and double components
			istringstream sin(line);
			sin >> name >> score;
			sin.clear();

			//puts name and score in a vector and then loops to ensure it isn't a duplicate, adding scores of same person
			vectorName.push_back(name);
			vectorScore.push_back(score);
			for (unsigned int i = 0; i < vectorName.size(); i++) {
				if (name == vectorName.at(i) && (i + 1) != vectorName.size()) {
					vectorName.erase(vectorName.begin() + (vectorName.size() - 1));
					vectorScore.erase(vectorScore.begin() + (vectorScore.size() - 1));
					vectorScore.at(i) = vectorScore.at(i) + score;
				}	
			}	
		}
		
		fin.close();

		//condenses vector down to only the amount of names desired by removing names and scores until reached names desired
		int intRemove = vectorName.size() - nameCount;	
		for (int i = 0; i < intRemove; i++) {
				vectorName.erase(vectorName.begin() + (vectorName.size() - 1));
				vectorScore.erase(vectorScore.begin() + (vectorScore.size() - 1));
		}
		
		//finds the character lenght of the longest name in the vector
		unsigned int maxNameWidth = vectorName.at(0).length();
		for (unsigned int i = 1; i < vectorName.size(); i++) {
			if (vectorName.at(i).length() > maxNameWidth) { 
				maxNameWidth = vectorName.at(i).length();
			}
		}
	
		//trunctes all the scores and puts them in a new vector of integers
		vector<int> intScores;
		for (unsigned int i = 0; i < vectorScore.size(); i++) {
			intScores.push_back(vectorScore.at(i));
		}

		//takes vector of integers and turns it into a vector of strings
		vector<string> vectorScoreString;
		string scoreString;
		for (unsigned int i = 0; i < intScores.size(); i++) {
			ostringstream sout;
			sout << intScores.at(i);
			scoreString = sout.str();
			vectorScoreString.push_back(scoreString);
			sout.str("");
		}

		//takes vector of score strings and finds the length of the longest (highest) score 
		unsigned int maxScoreWidth = vectorScoreString.at(0).length();
		for (unsigned int i = 1; i < vectorScoreString.size(); i++) {
			if (vectorScoreString.at(i).length() > maxScoreWidth) {
				maxScoreWidth = vectorScoreString.at(i).length();
			}
		}
		
		//prints out names and total scores in with no decimals and formatted
		for (unsigned int i = 0; i < vectorName.size(); i++){
			cout << left << setw(maxNameWidth) << vectorName.at(i) << " ";
			cout << right << setw(maxScoreWidth) << fixed << setprecision(0) << vectorScore.at(i) << '\n';
		}
	
	}
	return 0;
}
