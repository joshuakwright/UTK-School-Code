//Joshua Wright Cosc360 Lab2 Part 3
//This program reads in data from binary file "converted" and then ouputs all names matching the user input

#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include "fields.h"
#include "jval.h"
#include "dllist.h"
#include "jrb.h"

//struct for storing each machine entry
typedef struct {
  unsigned char address[4];
  Dllist names;
} IP;

int main(int argc, char **argv) {

    IP *ip;
    JRB rnode, machines = make_jrb();
    int in, size, pointer = 0;
    char buff[350000];

    //reading in all data from converted
    in = open("converted", O_RDONLY);
    size = read(in, buff, 350000);

    //this loop extracts all data from the buff array
    while(pointer < size) {
        ip = (IP*)malloc(sizeof(IP));
        //extracting IP address
        for (int i = 0; i < 4; i++) {
            ip->address[i] = buff[pointer++];
        }

        //extracting number of names
        pointer += 3;
        int nameCount = buff[pointer];
        
        ip->names = new_dllist();
        char stopChar = '\0';
        int place = 0;
        int wordEnd = 1;

        //extracting each name from the buffer
        for (int j = 0; j < nameCount; j++) {
            char *name = (char*)malloc(sizeof(char)*50);
            while(buff[pointer] != '\0') {
                if (wordEnd) pointer++;
                wordEnd = 1;
                //extracting each local name from the absolute path
                if (buff[pointer] == '.' && stopChar != '.') {
                    char *localName = (char*)malloc(sizeof(char)*50);
                    stopChar = '.';
                    strcpy(localName, name);
                    dll_append(ip->names, new_jval_s(localName));
                }
                name[place++] = buff[pointer];

            }
            wordEnd = 0;
            place = 0;
            stopChar = '\0';
            pointer++;
            dll_append(ip->names, new_jval_s(name));
        }

        //populates RB tree with all names 
        Dllist temp;
        dll_traverse(temp, ip->names) {
            rnode = jrb_find_str(machines, jval_s(temp->val));
            if (rnode == NULL) {
                Dllist insertTemp = new_dllist();
                dll_append(insertTemp, new_jval_v(ip));
                jrb_insert_str(machines, jval_s(temp->val), new_jval_v(insertTemp));
            }
            else {
                dll_append((Dllist)rnode->val.v, new_jval_v(ip));
            }
        }
    }
    printf("Hosts all read in\n\n");

    //takes in user input and outputs all matching machine names
    char name[50];
    printf("Enter host name: ");
    scanf("%s", name);
    while (!feof(stdin)) {
        rnode = jrb_find_str(machines, name);
        if (rnode == NULL) {
            printf("no key %s\n\n", name);
        }
        else {
            Dllist tempNames;
            Dllist names = rnode->val.v;
            dll_traverse(tempNames, names) {
                Dllist tempName;
                ip = (IP *)jval_v(tempNames->val);
                printf("%d.%d.%d.%d: ", ip->address[0], ip->address[1], ip->address[2], ip->address[3]);
                dll_traverse(tempName, ip->names) {
                    printf("%s ", jval_s(tempName->val));
                }
                printf("\n\n");
            }
        }
        printf("Enter host name: ");
        scanf("%s", name);
    }
    
    close(in);
    return 0;
}