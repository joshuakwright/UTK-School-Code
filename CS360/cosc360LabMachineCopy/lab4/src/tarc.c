//Joshua Wright CS360 Lab4 - Tarc Huang
//This is a basic reacreation of the tar create fucntionality

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "fields.h"
#include "jval.h"
#include "dllist.h"
#include "jrb.h"

/**
* @name InodeComp.
* @brief Comparison function for two void * jvals that hold unsigned longs.
* @param[in] i The first jval to compare.
* @param[in] j The second jval to compare.
* @return The result of a value comparision between i and j (-1 means i < j, 1 means i > j, 0 means they are equal).
*/
int
InodeComp(Jval i, Jval j);

/**
* @name ExtractDirectory.
* @brief Exctracts the relative directory from the absolute path.
* @param[in] directory The absolute path.
* @param[in] workingDir A pointer to the place in memory where the relative path will be stored.
*/
void
ExtractDirectory(char *directory, char *workingDir);

/**
* @name FreeJRBTree.
* @brief Frees the given jrb tree and its memebers based on what type of data is being stored.
* @param[in] tree The tree to be freed
* @param[in] type The type of data to be freed (void* or char*).
*/
void
FreeJRBTree(JRB tree, char type);

/**
* @name DirectoryParse.
* @brief Opens the working directory, reads in each file, and calls the print function for each.
* @param[in] directory The current working directory.
* @param[in] inodes The list of already seen inodes.
* @param[in] parentDir The parent of the current working directory
* @param[in] relativeDir The relative path to the current working directory.
*/
void
DirectoryParse(char *directory, JRB inodes, char *parentDir, char *relativeDir);

/**
* @name PrintFiles.
* @brief Calls all the functions needed to print each file and directory and calls the Directory parse frunction on directories.
* @param[in] directory The current working directory.
* @param[in] file The file to be printed.
* @param[in] type The type of file.
* @param[in] inodes The list of already seen inodes.
* @param[in] seen A boolean variable to tell if the current file has been seen before (0 for no and 1 for yes).
* @param[in] parentDir The parent of the current working directory
* @param[in] relativeDir The relative path to the current working directory.
*/
void
PrintFiles(char *directory, char *file, int type, JRB inodes, int seen, char *parentDir, char *relativeDir);

/**
* @name PrintPrimaryDir.
* @brief Prints the user inputted directory name and information.
* @param[in] directory The user inputted directory.
*/
void
PrintPrimaryDir(char *directory);

/**
* @name PrintFileNameSize.
* @brief Prints the size of a files name.
* @param[in] parentDir The parent directory of the current working directory.
* @param[in] fileName The name of the file to be printed.
*/
void
PrintFileNameSize(char *parentDir, char *fileName);

/**
* @name PrintFileName.
* @brief Prints the files name.
* @param[in] parentDir The parent directory of the current working directory.
* @param[in] fileName The name of the file to be printed.
*/
void
PrintFileName(char *parentDir, char *fileName);

/**
* @name PrintFileInode.
* @brief Prints the inode of a file.
* @param[in] buf The buffer conatining all the information about the current file.
*/
void
PrintFileInode(struct stat buf);

/**
* @name PrintFileMode.
* @brief Prints the mode of a file.
* @param[in] buf The buffer conatining all the information about the current file.
*/
void
PrintFileMode(struct stat buf);

/**
* @name PrintFileModTime.
* @brief Prints the last modification time of a file.
* @param[in] buf The buffer conatining all the information about the current file.
*/
void
PrintLastModTime(struct stat buf);

/**
* @name PrintFileSize.
* @brief Prints the size of a file.
* @param[in] buf The buffer conatining all the information about the current file.
*/
void
PrintFileSize(struct stat buf);

/**
* @name PrintFile.
* @brief Prints the contents of a file.
* @param[in] file The file to be opened and its contents printed.
* @param[in] buf The buffer conatining all the information about the current file.
*/
void
PrintFile(char *file, struct stat buf);

int 
main(int argc, char* argv[]) {
    JRB inodes = make_jrb();

    char *parentDir = malloc(strlen(argv[1])+2);   
    strcpy(parentDir, argv[1]);
    PrintPrimaryDir(parentDir);
    strcat(parentDir, "/");
    
    char *relativeDir = malloc(strlen(argv[1]));
    ExtractDirectory(argv[1], relativeDir);
    
    DirectoryParse(argv[1], inodes, parentDir, relativeDir);
    free(parentDir);
    free(relativeDir);
    FreeJRBTree(inodes, 'v');
    
    return 0;
}

int
InodeComp(Jval i, Jval j) {
    if (*((unsigned long *)i.v) < *((unsigned long *)j.v)) return -1;
    if (*((unsigned long *)i.v) > *((unsigned long *)j.v)) return 1;
    return 0;
}

void
ExtractDirectory(char *directory, char *workingDir) {
    int len = strlen(directory);
    int i;
    for (i = len; i > 0; i--) {
        if (directory[i] == '/') {
            i++;
            break;
        }
    }
    
    for(int j = i, k = 0; j < len+1; j++, k++) {
        workingDir[k] = directory[j];
    }
}

void
FreeJRBTree(JRB tree, char type) {
    JRB rnode;
    jrb_traverse(rnode, tree) {
        if (type == 'v') free(rnode->key.v);
        if (type == 's') free(rnode->key.s);
    }
    jrb_free_tree(tree);
}

void
DirectoryParse(char *directory, JRB inodes, char *parentDir, char *relativeDir) {
    if (directory[strlen(directory)-1] == '.') return;
    
    DIR *dir;
    struct dirent *dp;
    struct stat buf;

    if ((dir = opendir(directory)) == NULL) {
        fprintf(stderr, "Cannot open %s\n", directory);
        exit(1);
    }

    JRB nextDirs = make_jrb();

    chdir(directory);
    while ((dp = readdir(dir)) != NULL) {
        if (dp->d_type != 10 && strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
            stat(dp->d_name, &buf);
            //populates seen inode tree and prints if it's not a directory
            if (jrb_find_gen(inodes, new_jval_v(&buf.st_ino), InodeComp) == NULL) {
                void *temp = malloc(sizeof(unsigned long));
                memcpy(temp, &buf.st_ino, sizeof(unsigned long));
                jrb_insert_gen(inodes, new_jval_v(temp), JNULL, InodeComp);
                if (dp->d_type == 4) jrb_insert_str(nextDirs, strdup(dp->d_name), JNULL);
                else PrintFiles(directory, dp->d_name, dp->d_type, inodes, 0, parentDir, relativeDir);
            }
            else {
                if (dp->d_type != 4) PrintFiles(directory, dp->d_name, dp->d_type, inodes, 1, parentDir, relativeDir);
            }
        }
    }
    closedir(dir);

    //prints sub directorys of most recently opened directory
    JRB rnode;
    jrb_traverse(rnode, nextDirs) {
        PrintFiles(directory, rnode->key.s, 4, inodes, 0, parentDir, relativeDir);
    }
    FreeJRBTree(nextDirs, 's');
    chdir("..");

    return;
}

void
PrintFiles(char *directory, char *file, int type, JRB inodes, int seen, char *parentDir, char *relativeDir) {
    struct stat buf;
    stat(file, &buf);

    char *nRelDir = malloc(strlen(relativeDir)+2);
    strcpy(nRelDir, relativeDir);
    strcat(nRelDir, "/");
    PrintFileNameSize(nRelDir, file);
    free(nRelDir);
    
    PrintFileName(relativeDir, file);
    PrintFileInode(buf);
    
    //if the file is a directory
    if (type == 4) {
        PrintFileMode(buf);
        PrintLastModTime(buf);

        char *nextDir = malloc(strlen(parentDir)+strlen(file)+2);
        strcpy(nextDir, parentDir);
        strcat(nextDir, "/");
        strcat(nextDir, file);

        char *nextRelativeDir = malloc(strlen(relativeDir)+strlen(file)+2);
        strcpy(nextRelativeDir, relativeDir);
        strcat(nextRelativeDir, "/");
        strcat(nextRelativeDir, file);
        
        DirectoryParse(file, inodes, nextDir, nextRelativeDir);

        free(nextDir);
        free(nextRelativeDir);
    }
    //if the file is not a directory and this its first time being printed
    else if (seen == 0) {
        PrintFileMode(buf);
        PrintLastModTime(buf);
        PrintFileSize(buf);
        PrintFile(file, buf);
    }
}

void
PrintPrimaryDir(char *directory) {
    char *relativeDir = malloc(strlen(directory));
    ExtractDirectory(directory, relativeDir);
    PrintFileNameSize(relativeDir, "");
    fwrite(relativeDir, 1, strlen(relativeDir), stdout);
    free(relativeDir);

    struct stat buf;
    stat(directory, &buf);
    PrintFileInode(buf);
    PrintFileMode(buf);
    PrintLastModTime(buf);
}

void
PrintFileNameSize(char *parentDir, char *fileName) {
    char *nameSize = malloc(4);
    int len = (int)strlen(fileName) + (int)strlen(parentDir);
    nameSize[0] = len & 0xff;
    nameSize[1] = (len >> 8) & 0xff;
    nameSize[2] = (len >> 16) & 0xff;
    nameSize[3] = (len >> 24) & 0xff;
    fwrite(nameSize, 1, 4, stdout);
    free(nameSize);
}

void
PrintFileName(char *relativeDir, char *fileName) {
    char *printName = malloc(strlen(relativeDir)+ strlen(fileName)+2);
    strcpy(printName, relativeDir);
    strcat(printName, "/");
    strcat(printName, fileName);
    fwrite(printName, 1, strlen(printName), stdout);
    free(printName);
}

void
PrintFileInode(struct stat buf) {
    char *inode = malloc(8);
    inode[0] = buf.st_ino & 0xff;
    for (int i = 1; i < 8; i++) {
        inode[i] = (buf.st_ino >> 8*i) & 0xff;
    }
    fwrite(inode, 1, 8, stdout);
    free(inode);
}

void
PrintFileMode(struct stat buf) {
    char *mode = malloc(4);
    mode[0] = buf.st_mode & 0xff;
    mode[1] = (buf.st_mode >> 8) & 0xff;
    mode[2] = (buf.st_mode >> 16) & 0xff;
    mode[3] = (buf.st_mode >> 24) & 0xff;
    fwrite(mode, 1, 4, stdout);
    free(mode);
}

void
PrintLastModTime(struct stat buf) {
    char *modTime = malloc(8);
    modTime[0] = buf.st_mtime & 0xff;
    for (int i = 1; i < 8; i++) {
        modTime[i] = (buf.st_mtime >> 8*i) & 0xff;
    }
    fwrite(modTime, 1, 8, stdout);
    free(modTime);
}

void
PrintFileSize(struct stat buf) {
    char *fileSize = malloc(8);
    fileSize[0] = buf.st_size & 0xff;
    for (int i = 1; i < 8; i++) {
        fileSize[i] = (buf.st_size >> 8*i) & 0xff;
    }
    fwrite(fileSize, 1, 8, stdout);
    free(fileSize);
}

void
PrintFile(char *file, struct stat buf) {
        FILE *fp;
        char *buffer = malloc(buf.st_size);
        fp = fopen(file, "r");
        fread(buffer, 1, buf.st_size, fp);
        fwrite(buffer, 1, buf.st_size, stdout);
        fclose(fp);
        free(buffer);
}