//Joshua Wright Cosc360 Lab2 Part 1
//This program reads in data from binary file "converted" and then ouputs all names matching the user input

#include <stdlib.h>
#include <string.h>
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

    FILE  *in;
    IP *ip;
    JRB rnode, machines = make_jrb();
    int i = 1;

    in = fopen("converted", "r");\

    //this loop reads in all data from converted
    while(1) {
        //reading in the ip address
        ip = (IP*)malloc(sizeof(IP));
        i = fread(ip->address, sizeof(char), 4, in);

        if (feof(in)) break;

        //reading in the number of names
        unsigned char nameCountRead[4];
        i = fread(nameCountRead, sizeof(char), 4, in);
        int nameCount = nameCountRead[3];
        
        ip->names = new_dllist();
        char stopChar = '\0';
        int place = 0;

        //reading in each name
        for (int j = 0; j < nameCount; j++) {
            char *name = (char*)malloc(sizeof(char)*50);
            while(i != '\0') {
                i = fgetc(in);
                //extracts local name from absolute
                if (i == '.' && stopChar != '.') {
                    char *localName = (char*)malloc(sizeof(char)*50);
                    stopChar = '.';
                    strcpy(localName, name);
                    dll_append(ip->names, new_jval_s(localName));
                }
                name[place++] = i;
            }

            place = 0;
            stopChar = '\0';
            dll_append(ip->names, new_jval_s(name));
            i = 1;
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
    
    fclose(in);
    return 0;
}