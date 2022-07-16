/*
 * Doowon Kim
 * doowon@utk.edu
 * This is your simple program, called giveaway.c, that takes 
 * a customer's name as in the input and prints a giveaway for
 * the customer.
 * 
 * NOTE: You are not allowed to change this code at all.
 */

#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void giveaway(char* name){
	char name2[16];
	char* stuff[4] = {"Tesla S", "iPhone 12", "MacBook Pro 16", "Samsung Galaxy 21"};

	strcpy(name2, name); //hint: smash this
	printf("Customer name: %s:\n", name2);
	printf("Giveaway: %s!\n", stuff[rand()%4]);
	printf("Rand number: %d\n\n", rand());
}

int main(int argc, char ** argv){
	if (argc < 2) {
		printf("usage: %s customer name\n", argv[0]);
		return 0;
	}

	srand(time(0));
	giveaway(argv[1]);

	return 0;
}
