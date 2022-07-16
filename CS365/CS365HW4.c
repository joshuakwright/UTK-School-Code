#include <stdio.h>


struct node {
        int val;
        node *next;
};

node *head;

void
AddNode(node *head, int newNum) {
    node *newNode;
    newNode->val = newNum;
    newNode->next = NULL;

    if (head->next == NULL){
        head->next = newNode;
        return;
    }
    
    node *prev = head;
    node *temp = head->next;
    while(temp != NULL) {
        if (newNode < temp->val) {
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
PrintList(node *head) {
    printf("The sorted list:\n");

    head = head->next;
    while (head != NULL) {
        printf("%d\n", head->val);
        head = head->next;
    }
    printf("End of list\n");
    return;
}

int
main(int argc, char *argv) {
    //Change if don't need X at end
    if (argc != 1) {
        perror("Input error\n");
        exit(1);
    }

    printf("Input list of numbers separated by whitespace that's terminated by an X\n");

    head->val = 0;
    head->next = NULL;

    char input = '\0';
    int num = 0;
    while (input != 'X') {
        if (input >= 48 && input <= 57) {
            num *= 10;
            num += atoi(input);
        }
        else {
            AddNode(head, num);
            num = 0;
        } 
        input = getc(stdin);
    }

    PrintList(head);
    return 0;
}