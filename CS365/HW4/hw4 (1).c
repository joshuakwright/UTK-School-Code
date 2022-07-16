//Joshua Wright jwrig117

#include <stdio.h>
#include <stdlib.h>

struct node {
        int val;
        struct node *next;
};

struct node *head;

void
AddNode(int newNum) {
    struct node *newNode = malloc(sizeof(struct node *));
    newNode->val = newNum;
    newNode->next = NULL;

    if (head->next == NULL){
        head->next = newNode;
        return;
    }
    
    struct node *prev = head;
    struct node *temp = head->next;
    while(temp != NULL) {
        if (newNum < temp->val) {
            prev->next = newNode;
            newNode->next = temp;
            return;
        }
        prev = temp;
        temp = temp->next;
    }

    prev->next = newNode;
    return;
}

void
PrintList() {
    printf("\nThe sorted list is the following:\n");

	struct node *prev = head;
    head = head->next;
	free(prev);
    while (head != NULL) {
        printf("%d\n", head->val);
        prev = head;
		head = head->next;
		free(prev);
    }
    printf("End of list\n");
    return;
}

int
main(int argc, char *argv) {
    if (argc != 1) {
        perror("Input error\n");
        exit(1);
    }

    printf("Input list of numbers separated by whitespace that's terminated by an X\n");

	head = malloc(sizeof(struct node *));
    head->val = 0;
    head->next = NULL;

    char input = '\0';
    int num = 0;
    while (input != 'X') {
        if (input >= 48 && input <= 57) {
            num *= 10;
            num += input - '0';
        }
        else if (input != '\0' && num != 0) {
            AddNode(num);
            num = 0;
        } 
        input = getc(stdin);
    }

    PrintList();
    return 0;
}
