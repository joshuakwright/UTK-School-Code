//main.cpp
//This lab
//Joshua Wright
//15-Oct-2019
//Professor: Dr.Marz
//Students worked with:
//None
#include <iostream>
#include <cstdlib>
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
using namespace std;

//Function prototypes
int GetRandom(int seed, int min, int max);

int DealCard(int (&cardArray)[52], int playerNumber);

int ScoreHand(int (&cardArray)[52], int playerNumber);

string CardString(int playerCardIndex);

int main(int argc, char*argv[]) {

	//populates array with all 0s
	int cardArray[52];
	for (int i = 0; i < 52; i++) {
		cardArray[i] = 0;			
	}
	
	//variable declrations
	int playerNumber = 1;
	int playerCardIndex1;
	int playerCardIndex2;
	string playerCard1;
	string playerCard2;
	string playerCards;
	int playerCount;
	int playerScore;
	int seed;
	
	//Command line argument amount error check
	if (argc > 3 || argc < 3) {
		cout << "Incorrect arguments.\n";
		return 0;
	}

	//Gets seed value from command line input and error checks it
	istringstream sin(argv[1]);
	sin >> seed;
	if (sin.fail()) {
		cout << "Invalid seed.\n";
		return 0;
	}
	sin.clear();

	//Gets total players from command line and error checks it
	sin.str(argv[2]);
	sin >> playerCount;
	if (sin.fail() || playerCount > 25 || playerCount < 1) {
		cout << "Invalid number of players.\n";
		return 0;
	}
	sin.clear();
	//seeds rand()
	srand(seed);

	//For loop that outputs requested amount of players, cards, and score by refrencing four different functions as defined below
	for (int i = 0; i < playerCount; i++) {
		//function refrences
		playerCardIndex1 = DealCard(cardArray, playerNumber); 
		playerCardIndex2 = DealCard(cardArray, playerNumber);
		playerCard1 = CardString(playerCardIndex1);
		playerCard2 = CardString(playerCardIndex2);
		playerScore = ScoreHand(cardArray, playerNumber);
		//takes two player cards and puts them into one string with a space in the middle
		ostringstream sout;
		sout << playerCard1 << " " << playerCard2;
		playerCards = sout.str();
		sout.str("");
		//outputs player, card, and score
		cout << "Player " << right <<  setw(2) <<  playerNumber; 
		cout << " has cards: " << left << setw(20) << playerCards; 
		cout << "(" << playerScore << ")\n";

		playerNumber++;
	}

	return 0;

}
//random number function definition
int GetRandom(int min, int max) {
	
	int randomNumber;
	//recalculates random number to fit into the needed refrence frame
	randomNumber = min + rand() % (max - min + 1);
	return randomNumber;
}

//deals a card to the selected player
int DealCard(int (&cardArray)[52], int playerNumber) {
	
	int min = 0;
	int max = 51;
	bool boolean = true;
	int index;
	//this is a while loop that will continue to draw cards until a card that has not been picked yet is drawn
	do {
	index = GetRandom(min, max); 
	if (cardArray[index] == 0) {
		cardArray[index] = playerNumber;
		boolean = false;
		}
	} while (boolean);
	return index;
}

//function definition that scores the hand of each player
int ScoreHand(int (&cardArray)[52], int playerNumber) {

	//sorts throught the array of cards and puts the indexes of the cards of the selected player into a vector
	vector<int> tempHandScore;
		for (int i = 0; i < 52; i++) {
			if (cardArray[i] == playerNumber) {
				tempHandScore.push_back(i);
			}
		}
	//calculates the score of card 1
	int card1Score = (tempHandScore.at(0) / 4) + 1;
	if (card1Score > 10) {
		card1Score = 10;
	}
	//calculates the score of card 2
	int card2Score = (tempHandScore.at(1) / 4) + 1;
	if (card2Score > 10) {
		card2Score = 10;
	}
	//adds the score of the two cards to get a total score
	int totalScore = card1Score + card2Score;
	return totalScore;
}

//function definition that outputs a string that represents a card's number and suit
string CardString(int playerCardIndex) {
	
	string number[] {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
	string suit[] {"D", "H", "C", "S"};

	//calculates the number and suit based off the cardArray index and then assigns them to a string variable
	string cardNumber = number[(playerCardIndex / 4)];
	string cardSuit = suit[(playerCardIndex % 4)];
	//takes the number and suit strings and puts them together into one string
	string card;
	ostringstream sout;
	sout << cardNumber << cardSuit;
	card = sout.str();
	sout.str("");

	return card;

}
