#include <stdlib.h>
#include <string.h>
#include "fields.h"
#include "jval.h"
#include "dllist.h"
#include "jrb.h"

typedef struct {
    char *name;
    char *sex;
    char *father;
    char *mother;
    Dllist children;
    int printed;
    int visited;
} person;

/**
* @name ExtractName.
* @brief Extracts the name from each line.
* @param[in] fields :The array of names to be put together.
* @param[in] size : How many words are in the fields array.
* @return The full name bound together.
*/
char *
ExtractName(char **fields, int size) {
    int nameSize = 0;
    for(int i = 1; i < size; i++) {
        nameSize += strlen(fields[i]);
        if (i < size) {
            nameSize++;
        }
    }
    char *tempName = malloc(nameSize);
    for(int i = 1; i < size; i++) {
        strcat(tempName, fields[i]);
        if (i + 1 < size) {
            strcat(tempName, " ");
        }
    }
    
    return tempName;
}

/**
* @name AddPerson.
* @brief Adds a person to the red black tree of people.
* @param[in] people : the red black tree the person is to be added to.
* @param[in] name : name of the person to be added.
*/
void
AddPerson(JRB people, char *name) {
    JRB tempNode;
    person *personInfo;

    tempNode = jrb_find_str(people, name);
    if (tempNode == NULL) {
        personInfo = (person *) malloc(sizeof(person));
        personInfo->name = name;
        personInfo->visited = 0;
        personInfo->printed = 0;
        jrb_insert_str(people, name, new_jval_v(personInfo));
    }
}

/**
* @name CheckRelation.
* @brief Checks to see if there is an existing relation between two people and outputs an error if there is a relationship conflict.
* @param[in] people : the red black tree containing all the people.
* @param[in] name : name of the current header person (as in the last person to be on a PERSON line in the read in file).
* @param[in] relative : the name of the relative under the header person
* @param[in] realtion : the realationship between the header person and the relative
* @return returns 0 if there is no relationship or the relationship is invalid and 1 if the relationship is existing and valid.
*/
int
CheckRelation(JRB people, char *name, char *relative, char *relation) {
    JRB tempNode;
    person *personInfo;

    if (strcmp(relation, "FATHER") == 0) {
        tempNode = jrb_find_str(people, name);
        personInfo = (person *)tempNode->val.v;
        if (personInfo->father == NULL) return 0;
        if(strcmp(personInfo->father, relative) == 0) {
            return 1;
        }
        fprintf(stderr, "Incorrect linkage between %s and %s\n", name, relative);
        return 0;
    }

    if (strcmp(relation, "MOTHER") == 0) {
        tempNode = jrb_find_str(people, name);
        personInfo = (person *)tempNode->val.v;
        if (personInfo->mother == NULL) return 0;
        if(strcmp(personInfo->mother, relative) == 0) {
            return 1;
        }
        fprintf(stderr, "Incorrect linkage between %s and %s\n", name, relative);
        return 0;
    }

    if (strcmp(relation, "FATHER_OF") == 0) {
        tempNode = jrb_find_str(people, relative);
        personInfo = (person *)tempNode->val.v;
        if (personInfo->father == NULL) return 0;
        if(strcmp(personInfo->father, name) == 0) {
            return 1;
        }
        fprintf(stderr, "Incorrect linkage between %s and %s\n", name, relative);
        return 0;
    }

    if (strcmp(relation, "MOTHER_OF") == 0) {
        tempNode = jrb_find_str(people, relative);
        personInfo = (person *)tempNode->val.v;
        if (personInfo->mother == NULL) return 0;
        if(strcmp(personInfo->mother, name) == 0) {
            return 1;
        }
        fprintf(stderr, "Incorrect linkage between %s and %s\n", name, relative);
        return 0;
    }
}

/**
* @name CreateLink.
* @brief Creates a relationship link going both ways between the two people in the people tree.
* @param[in] people : the red black tree containing all the people.
* @param[in] name : name of the current header person (as in the last person to be on a PERSON line in the read in file).
* @param[in] relative : the name of the relative under the header person
* @param[in] realtion : the realationship between the header person and the relative
*/
void
CreateLink(JRB people, char *name, char *relative, char *relation) {
    JRB tempNode;
    person *personInfo;
    
    if (strcmp(relation, "FATHER") == 0) {
        tempNode = jrb_find_str(people, name);
        personInfo = (person *)tempNode->val.v;
        personInfo->father = malloc(strlen(relative));
        strcat(personInfo->father, relative);

        tempNode = jrb_find_str(people, relative);
        personInfo = (person *)tempNode->val.v;
        if (personInfo->children == NULL) {
            personInfo->children = new_dllist();
        }
        dll_append(personInfo->children, new_jval_s(name));
    }

    if (strcmp(relation, "MOTHER") == 0) {
        tempNode = jrb_find_str(people, name);
        personInfo = (person *)tempNode->val.v;
        personInfo->mother = malloc(strlen(relative));
        strcat(personInfo->mother, relative);

        tempNode = jrb_find_str(people, relative);
        personInfo = (person *)tempNode->val.v;
        if (personInfo->children == NULL) {
            personInfo->children = new_dllist();
        }
        dll_append(personInfo->children, new_jval_s(name));
    }

    if (strcmp(relation, "FATHER_OF") == 0) {
        tempNode = jrb_find_str(people, relative);
        personInfo = (person *)tempNode->val.v;
        personInfo->father = malloc(strlen(name));
        strcat(personInfo->father, name);

        tempNode = jrb_find_str(people, name);
        personInfo = (person *)tempNode->val.v;
        if (personInfo->children == NULL) {
            personInfo->children = new_dllist();
        }
        dll_append(personInfo->children, new_jval_s(relative));
    }

    if (strcmp(relation, "MOTHER_OF") == 0) {
        tempNode = jrb_find_str(people, relative);
        personInfo = (person *)tempNode->val.v;
        personInfo->mother = malloc(strlen(name));
        strcat(personInfo->mother, name);

        tempNode = jrb_find_str(people, name);
        personInfo = (person *)tempNode->val.v;
        if (personInfo->children == NULL) {
            personInfo->children = new_dllist();
        }
        dll_append(personInfo->children, new_jval_s(relative));
    }
}

/**
* @name AssumeSex.
* @brief Checks to see if there is an existing relation between two people and outputs an error if there is a relationship conflict.
* @param[in] people : the red black tree containing all the people.
* @param[in] name : name of the person to assign a sex to.
* @param[in] realtion : the realationship being used to judge which sex the person should be
*/
void 
AssumeSex(JRB people, char *name, char *relation) {
    person *personInfo;
    char *temp = malloc(1);
    JRB tempNode = jrb_find_str(people, name);
    personInfo = (person *)tempNode->val.v;
    if (personInfo->sex != NULL) return;
    if(strcmp(relation, "FATHER") == 0) strcat(temp, "M");
    else if(strcmp(relation, "FATHER_OF") == 0) strcat(temp, "M");
    else if(strcmp(relation, "MOTHER") == 0) strcat(temp, "F");
    else if(strcmp(relation, "MOTHER_OF") == 0) strcat(temp, "F");

    personInfo->sex = temp;   
}

/**
* @name CycleCheck.
* @brief Checks for cycles in the people tree.
* @param[in] people : the red black tree containing all the people.
* @param[in] personInfo : info containing the current person being checked for a cycle
* @return returns 1 if cycle is detected and 0 if none.
*/
int
CycleCheck(JRB people, person *personInfo) {
    Dllist tempList;
    person *tempPerson;
    JRB tempNode;

    if (personInfo->visited == 1) return 0;

    if (personInfo->visited == 2) return 1;
    
    personInfo->visited = 2;
    if (personInfo->children != NULL) {
        dll_traverse(tempList, personInfo->children) {
            tempNode = jrb_find_str(people, jval_s(tempList->val));
            tempPerson = (person *)tempNode->val.v;
            if (CycleCheck(people, tempPerson)) return 1;
        }
    }
    personInfo->visited = 1;
    return 0;
}

/**
* @name PrintFamilyTree.
* @brief Prints the whole family tree in order of descendants.
* @param[in] people : the red black tree containing all the people.
*/
void
PrintFamilyTree(JRB people) {
    Dllist nil, tempList, toPrint = new_dllist();
    JRB tempNode;
    person *tempPerson, *tempFather, *tempMother, *tempChild;

    jrb_traverse(tempNode, people) {
        dll_append(toPrint, new_jval_v((person *)tempNode->val.v));
    }    

    while (!dll_empty(toPrint)) {
        tempPerson = (person *)jval_v(dll_first(toPrint)->val);
        
        toPrint->flink = toPrint->flink->flink;
        

        if (tempPerson->printed == 0) {
            if (tempPerson->father != NULL) {
                tempNode = jrb_find_str(people, tempPerson->father);
                tempFather = (person *)tempNode->val.v;
            }
            if (tempPerson->mother != NULL) {
                tempNode = jrb_find_str(people, tempPerson->mother);
                tempMother = (person *)tempNode->val.v;
            }
            if (!(tempPerson->father == NULL && tempPerson->mother == NULL) || ((tempPerson->father == NULL ? 0 : tempFather->printed == 1) && (tempPerson->mother == NULL ? 0 : tempMother->printed == 1)) || (tempPerson->father == NULL && tempPerson->mother != NULL && tempMother->printed == 1) || (tempPerson->mother == NULL && tempPerson->father != NULL && tempFather->printed == 1)) {
                dll_append(toPrint, new_jval_v(tempPerson));
            }
            if ((tempPerson->father == NULL && tempPerson->mother == NULL) || ((tempPerson->father == NULL ? 0 : tempFather->printed == 1) && (tempPerson->mother == NULL ? 0 : tempMother->printed == 1)) || (tempPerson->father == NULL && tempPerson->mother != NULL && tempMother->printed == 1) || (tempPerson->mother == NULL && tempPerson->father != NULL && tempFather->printed == 1)) {
                printf("%s\n", tempPerson->name);
                if(tempPerson->sex != NULL) {
                    if (strcmp(tempPerson->sex, "F") == 0) {
                        printf("  Sex: Female\n");
                    }
                    else {
                        printf("  Sex: Male\n");
                    }
                }   
                else {
                    printf("  Sex: Unknown\n");
                }
                if (tempPerson->father == NULL) {
                    printf("  Father: Unknown\n");
                }
                else {
                    printf("  Father: %s\n", tempPerson->father);
                }
                if (tempPerson->mother == NULL) {
                    printf("  Mother: Unknown\n");
                }
                else {
                    printf("  Mother: %s\n", tempPerson->mother);
                }
                if (tempPerson->children == NULL) {
                    printf("  Children: None\n");
                }
                else {
                    printf("  Children:\n");
                    dll_traverse(tempList, tempPerson->children) {
                        tempNode = jrb_find_str(people, jval_s(tempList->val));
                        tempChild = (person *)tempNode->val.v;
                        printf("    %s\n", tempChild->name);
                    }
                }

                printf("\n");
                tempPerson->printed = 1;
            }
        }
    }
}

int 
main(int argc, char **argv) {
    
    IS is;
    int i;
    JRB people, tempNode;
    people = make_jrb();
    char *name;
    person *personInfo;
    
    is = new_inputstruct(argv[1]);
    if (is == NULL) {
        perror(argv[1]);  
        exit(1);
    }
    
    while(get_line(is) >= 0) {
        if (strcmp(is->fields[0], "PERSON") == 0 && is->NF != 0) { 
            if (name == NULL || strlen(name) == 0) free(name);
            name = ExtractName(is->fields, is->NF);
            AddPerson(people, name);
        }

        else if (strcmp(is->fields[0], "SEX") == 0 && is->NF != 0) {
            tempNode = jrb_find_str(people, name);
            personInfo = (person *)tempNode->val.v;
            if (personInfo->sex != NULL && strcmp(personInfo->sex, is->fields[1]) != 0) {
                fprintf(stderr, "Bad input - sex mismatch on line %d\n", is->line);
            }
            personInfo->sex = malloc(1);
            strcat(personInfo->sex, is->fields[1]);
        }

        else if(strcmp(is->fields[0], "FATHER") == 0 && is->NF != 0) {
            char *tempName;
            tempName = ExtractName(is->fields, is->NF);
            AddPerson(people, tempName);
            AssumeSex(people, tempName, "FATHER");
            int i = CheckRelation(people, name, tempName, "FATHER");
            if(i == 0) {
                CreateLink(people, name, tempName, "FATHER");
            }
        }

        else if(strcmp(is->fields[0], "MOTHER") == 0 && is->NF != 0) {
            char *tempName;
            tempName = ExtractName(is->fields, is->NF);
            AddPerson(people, tempName);
            AssumeSex(people, tempName, "MOTHER");
            int i = CheckRelation(people, name, tempName, "MOTHER");
            if(i == 0) {
                CreateLink(people, name, tempName, "MOTHER");
            }
        }

        else if(strcmp(is->fields[0], "FATHER_OF") == 0 && is->NF != 0) {
            char *tempName;
            tempName = ExtractName(is->fields, is->NF);
            AddPerson(people, tempName);
            AssumeSex(people, name, "FATHER_OF");
            int i = CheckRelation(people, name, tempName, "FATHER_OF");
            if(i == 0) {
                CreateLink(people, name, tempName, "FATHER_OF");
            }
        }

        else if(strcmp(is->fields[0], "MOTHER_OF") == 0 && is->NF != 0) {
            char *tempName;
            tempName = ExtractName(is->fields, is->NF);
            AddPerson(people, tempName);
            AssumeSex(people, name, "MOTHER_OF");
            int i = CheckRelation(people, name, tempName, "MOTHER_OF");
            if(i == 0) {
                CreateLink(people, name, tempName, "MOTHER_OF");
            }
        }
    }


    jrb_traverse(tempNode, people) {
        if(CycleCheck(people, (person *)tempNode->val.v) == 1) {
            fprintf(stderr, "Bad input -- cycle in specification\n");
            exit(1);
        }
    }

    PrintFamilyTree(people);

    jettison_inputstruct(is);
    exit(0);
    
}