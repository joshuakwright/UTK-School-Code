//Joshua Wright CS360 Lab 4 Huang
//This is a basic recreation of the tar functionionality that takes a tar file and parses out
//its contents into their respective directories and files

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <utime.h>
#include "fields.h"
#include "jval.h"
#include "dllist.h"
#include "jrb.h"

typedef struct {
    unsigned int mode[1];
    struct timeval time[2];
} tmode;

/**
* @name InodeComp.
* @brief compares inode values for a JRB tree.
* @param[in] i A Jval containing the first inode to compare.
* @param[in] j A Jval containing the second inode to compare.
* @return The result of the inode comparison.
*/
int
InodeComp(Jval i, Jval j) {
    if (i.l < j.l) return -1;
    if (i.l > j.l) return 1;
    return 0;
}

/**
* @name ReadInFile.
* @brief Reads in the tar files and creates the directories and files contained therein and populates a 
* JRB tree of the directories for later use.
* @param[in] inodes A JRB tree to be used to keep track of which files have already been seen.
* @param[in] directories A JRB tree used to store all the directories and each's permission and time setting so that they can be set later.
* @return Returns 1 on a successful reading and 1 on EOF.
*/
int
ReadInFile(JRB inodes, JRB directories){
    int nameSize[1];
    int size = fread(nameSize, 1, 4, stdin);

    if (size == 0) return 0;
    if (size < 4) fprintf(stderr, "Tried to read filename size, but only got %i bytes.\n", size);

    char *fileName = malloc(nameSize[0]+1);
    size = fread(fileName, 1, nameSize[0], stdin);
    if (size < nameSize[0]) fprintf(stderr, "File name size is %i, but bytes read = %i.\n", nameSize[0], size);
    fileName[nameSize[0]] = '\0';

    long inode[1];
    size = fread(inode, 1, 8, stdin);
    if (size < 8) fprintf(stderr, "Couldn't read inode.\n");

    //checks to see if the file needs to be printed or a hardlink created
    JRB rnode = jrb_find_gen(inodes, new_jval_l(inode[0]), InodeComp);
    if (rnode != NULL) {
        link(rnode->val.s, fileName);
    }
    else {
        jrb_insert_gen(inodes, new_jval_l(inode[0]), new_jval_s(strdup(fileName)), InodeComp);

        unsigned int mode[1];
        size = fread(mode, 1, 4, stdin);
        if (size < 4) fprintf(stderr, "Couldn't read mode.\n");

        long modTime[1];
        fread(modTime, 8, 1, stdin);
        
        //either creates directory or file
        if (S_ISDIR(mode[0])) {
            tmode *timeMode = malloc(sizeof(tmode));
            timeMode->mode[0] = mode[0];
            timeMode->time[0].tv_sec = time(NULL);
            timeMode->time[0].tv_usec = 0;
            timeMode->time[1].tv_sec = modTime[0];
            timeMode->time[1].tv_usec = 0;
            jrb_insert_str(directories, strdup(fileName), new_jval_v(timeMode));
            
            mkdir(fileName, 0755);
            chmod(fileName, 0755);
        }
        else {
            long fileSize[1];
            size = fread(fileSize, 1, 8, stdin);
            if (size < 8) fprintf(stderr, "Couldn't read size.\n");

            char *fileBuf = malloc(fileSize[0]);
            size = fread(fileBuf, 1, fileSize[0], stdin);
            if (size < fileSize[0]) fprintf(stderr, "Trying to read %li bytes of the file, and got EOF\n", fileSize[0]);

            FILE *fp;
            fp = fopen(fileName, "w");
            fwrite(fileBuf, 1, fileSize[0], fp);
            fclose(fp);
            free(fileBuf);

            struct timeval times[2];
            times[0].tv_sec = time(NULL);
            times[0].tv_usec = 0;
            times[1].tv_sec = modTime[0];
            times[1].tv_usec = 0;
            utimes(fileName, times);
            chmod(fileName, mode[0]);
        }
    }
    free(fileName);

    return 1;
}

/**
* @name AddDirectoryPermissions.
* @brief Sets all of the directories permission and last modified times to that outlined in the tarc file.
* @param[in] directories A JRB tree containing all the directories and their permissions / last modified time.
*/
void
AddDirectoryPermissions(JRB directories) {
    JRB temp;
    jrb_traverse(temp, directories) {
        tmode *timeMode = temp->val.v;
        utimes(temp->key.s, timeMode->time);
        chmod(temp->key.s, timeMode->mode[0]);
    }
}

/**
* @name FreeMem.
* @brief Frees all the memory in the inodes and directories trees as well as the trees themselves.
* @param[in] inodes JRB tree to be freed.
* @param[in] directories JRB tree to be freed.
*/
void
FreeMem(JRB inodes, JRB directories) {
    JRB rnode;
    jrb_traverse(rnode, inodes) {
        free(rnode->val.s);
    }
    jrb_free_tree(inodes);

    jrb_traverse(rnode, directories) {
        free(rnode->key.s);
        free(rnode->val.v);
    }
    jrb_free_tree(directories);
}

int main(int argc, char* argv[]) {   
    JRB inodes = make_jrb();
    JRB directories = make_jrb();
    while (ReadInFile(inodes, directories)) {}
    AddDirectoryPermissions(directories);
    FreeMem(inodes, directories);

    return 0;
}