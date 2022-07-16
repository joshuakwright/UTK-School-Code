/**
 * Joshua Wright CS360 Lab 3 Huang
 * This program will take in a reduced form of a makefile and use it to compile the code as make would
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include "fields.h"
#include "jval.h"
#include "dllist.h"

/**
* @name FreeFullDllist.
* @brief Free's the strings in each entry of the dll and then the dll itself.
* @param[in] list: is a Dllist to be freed.
*/
void
FreeFullDllist(Dllist list) {
    Dllist temp;
    dll_traverse(temp, list) {
        free(temp->val.s);
    }
    free_dllist(list);
}


/**
* @name CFileStatus.
* @brief This will do an neccessary checks to see if any .o files need to be recompiled and then recompile them as needed.
* @param[in] cfiles: is a list of the .c files.
* @param[in] flags: is a list of flags to be used when compiling.
* @param[in] mostRecentHeader: is the time of the most recently edited header file used by any of the .c files.
* @param[out] maxTime: this is the time of the most recently compiled .o file.
* @return The most recent time of creation/alter of the compiled .o files.
*/
time_t
CFileStatus(Dllist cfiles, Dllist flags, time_t mostRecentHeader) {
    struct stat cbuf;
    struct stat obuf;
    int exists = 0;
    Dllist temp, temp2;
    time_t maxTime = 0;

    dll_traverse(temp, cfiles) {
        char *file = temp->val.s;
        exists = stat(file, &cbuf);
        if (exists < 0) {
            fprintf(stderr, "fmakefile: %s: No such file or directory\n", file);
            return -1;
        }
        else {
            int i = strlen(file);
            char *ofile = strdup(file);
            ofile[i-1] = 'o';
            exists = stat(ofile, &obuf);
            if (exists >= 0) {
                if (maxTime == 0) maxTime = obuf.st_mtime;
                if (obuf.st_mtime > maxTime) maxTime = obuf.st_mtime;
            }
            if (exists < 0 || obuf.st_mtime < cbuf.st_mtime || obuf.st_mtime < mostRecentHeader) {
                char compile[] = "gcc -c ";
                dll_traverse(temp2, flags) {
                    strcat(compile, temp2->val.s);
                    strcat(compile, " ");
                }
                strcat(compile, file);
                printf("%s\n", compile);
                if (system(compile) != 0) {
                    return -2;
                }
                stat(ofile, &obuf);
                maxTime = obuf.st_mtime;
            }
            free(ofile);
        }
    }
    return maxTime;
}


/**
* @name HeaderFileStatus.
* @brief This returns the most recent time that a header file was altered or created.
* @param[in] list: this is a list of header files to check.
* @param[out] maxTime: this is the time of the most recently edited/created header file.
* @return The time of the most recently edited/created header file.
*/
time_t
HeaderFileStatus(Dllist list) {
    struct stat buf;
    int exists = 0;
    Dllist temp;
    time_t maxTime = 0;
    
    dll_traverse(temp, list) {
        exists = stat(temp->val.s, &buf);
        if (exists < 0) {
            fprintf(stderr, "fmakefile: %s: No such file or directory\n", temp->val.s);
            return -1;
        }
        else {
            if(maxTime == 0) maxTime = buf.st_mtime;
            if(buf.st_mtime > maxTime) maxTime = buf.st_mtime;
        }
    }

    return maxTime;
}

int main(int argc, char **argv) {
     
    IS is;
    int i;
    char *file;

    if (argc == 2) {
        file = strdup(argv[1]);
    }
    else {
        file = strdup("fmakefile");
    }
    is = new_inputstruct(file);
    
    if(is == NULL) {
        perror(argv[1]);
        exit(1);
    }

    Dllist cfiles = new_dllist();
    Dllist hfiles = new_dllist();
    Dllist flags = new_dllist();
    Dllist libraries = new_dllist();
    Dllist temp;
    int execFound = 0;
    int exists;
    int line = 1;
    time_t htime;
    char *executable;
    struct stat buf;

    //reads in file and parses contents into appropriate Dllists
    while(get_line(is) >= 0) {
        for(i = 0; i < is->NF; i++) {
            if (!strcmp(is->fields[0], "C") && i > 0) {
                dll_append(cfiles, new_jval_s(strdup(is->fields[i])));
            }
            else if (!strcmp(is->fields[0], "H") && i > 0) {
                dll_append(hfiles, new_jval_s(strdup(is->fields[i])));
            }
            else if (!strcmp(is->fields[0], "L") && i > 0) {
                dll_append(libraries, new_jval_s(strdup(is->fields[i])));
            }
            else if (!strcmp(is->fields[0], "E") && i > 0) {
                if (execFound) {
                    fprintf(stderr, "fmakefile (%d) cannot have more than one E line\n", line);
                    exit(1);
                }
                executable = strdup(is->fields[i]);
                execFound = 1;
            }
            else if (!strcmp(is->fields[0], "F") && i > 0) {
                dll_append(flags, new_jval_s(strdup(is->fields[i])));
            }
        }
        line++;
    }

    //checks to see if an executable file was found
    if(!execFound) {
        fprintf(stderr, "No executable specified\n");
        exit(1);
    }

    //check for missing header file
    htime = HeaderFileStatus(hfiles);
    if (htime == -1) exit(1);

    //checks for missing file and failed system call
    time_t mostRecentOFile = CFileStatus(cfiles, flags, htime);
    if (mostRecentOFile == -1) exit(1);
    if (mostRecentOFile == -2) {
        fprintf(stderr, "Command failed.  Exiting\n");
        exit(1);
    }

    //builds and runs final system call to compile executable
    exists = stat(executable, &buf);
    if (exists < 0 || mostRecentOFile == 0 || mostRecentOFile > buf.st_mtime) {
        char compile[] = "gcc";
        strcat(compile, " -o ");
        strcat(compile, executable);
        dll_traverse(temp, flags) {
            strcat(compile, " ");
            strcat(compile, temp->val.s);
        }
        dll_traverse(temp, cfiles) {
            char *file = temp->val.s;
            int i = strlen(file);
            char *ofile = strdup(file);
            ofile[i-1] = 'o';
            strcat(compile, " ");
            strcat(compile, ofile);
            free(ofile);
        }
        dll_traverse(temp, libraries) {
            strcat(compile, " ");
            strcat(compile, temp->val.s);
        }
        printf("%s\n", compile);
        if (system(compile) != 0) {
            fprintf(stderr, "Command failed.  Fakemake exiting\n");
            exit(1);
        }
    }
    else {
        printf("%s up to date\n", executable);
    }

    FreeFullDllist(cfiles);
    FreeFullDllist(hfiles);
    FreeFullDllist(libraries);
    FreeFullDllist(flags);
    free(executable);
    free(file);
    jettison_inputstruct(is);

    return 0;
 }