//Joshua Wright CS360 Huang Lab 8
//This is an program built to mimic the functionality of a terminal in exectuting commands and using any combination of <, >, >>, |.

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include "fields.h"
#include "jval.h"
#include "dllist.h"
#include "jrb.h"

int main(int argc, char** argv) {
    IS is;
    is = new_inputstruct(NULL);

    while(get_line(is) >= 0) {
        JRB pids = make_jrb();
        int read = -1, write = -1;
        int status;
        int fd[2]; 
        int pid;
        char noWait = ' ';
        char *lastWord = NULL;
        int tracker = 0;
        char **tempCmds;
        int size = is->NF;
        
        //Reading in lines
        if (strcmp(is->fields[size-1], "&") == 0) {
            noWait = '&';
            tempCmds = (char**)malloc(size*sizeof(char*));
            is->fields[size-1] = NULL;
        }
        else {
            tempCmds = (char**)malloc((size+1)*sizeof(char*));
            tempCmds[size] = NULL;
        }
        for (int i = 0; i < size; i++) {
            tempCmds[i] = is->fields[i];
        }
        
        //Open I/O files and do multiple forks with pipes if needed
        for (int i = 0; tempCmds[i] != NULL; i++) {
            //stdin
            if (strcmp(tempCmds[i], "<") == 0) {
                tempCmds[i] = NULL;
                read = open(tempCmds[++i], O_RDONLY);
                tempCmds[i] = NULL;
            }
            //stdout
            else if (strcmp(tempCmds[i], ">") == 0) {
                tempCmds[i] = NULL;
                lastWord = tempCmds[i+1];
                write = open(lastWord, O_WRONLY | O_TRUNC | O_CREAT, 0644);
            }
            //append
            else if (strcmp(tempCmds[i], ">>") == 0) {
                tempCmds[i] = NULL;
                write = open(tempCmds[++i], O_WRONLY | O_APPEND, 0644);
                tempCmds[i] = NULL;
            }
            //Do multiple forks and use pipes to pipe each command together
            else if (strcmp(tempCmds[i], "|") == 0) {
                tempCmds[tracker] = NULL;
                tracker = 0;

                if (pipe(fd) < 0) {
                    perror("error");
                    exit(1);
                }
                if (fork() == 0) {
                    if (read != -1) {
                        if(dup2(read, 0) != 0) {
                            perror("error");
                            exit(1);
                        }
                        close(read);
                        read = -1;
                    }
                    if(dup2(fd[1], 1) != 1) {
                        perror("error");
                        exit(1);
                    }
                    close(fd[0]);
                    close(fd[1]);

                    execvp(tempCmds[0], tempCmds);
                    perror(tempCmds[0]);
                    exit(1);
                }
                else {
                    if (read != -1) close(read);
                    read = fd[0];
                    close(fd[1]);
                }
            }
            else {
                tempCmds[tracker++] = is->fields[i];
            }
        }

        //Remove last word from command
        if (lastWord != NULL) {
            for (int i = 0; tempCmds[i] != NULL; i++) {
                if (tempCmds[i] == lastWord) tempCmds[i] = NULL;
            }
        }

        //Forks for command with no pipes or last in the pipeline

        //Waits for processes to finish
        if (noWait != '&') {
            if ((pid = fork()) == 0) {
                if (read != -1) dup2(read, 0);
                close(read);
                if (write != -1) dup2(write, 1);
                close(write);

                execvp(tempCmds[0], tempCmds);
                perror(tempCmds[0]);
                exit(1);
            }
            else {
                int currPid = wait(&status);
                while (currPid != pid) {
                    if (jrb_find_int(pids, currPid) != NULL) jrb_delete_node(jrb_find_int(pids, currPid));
                    currPid = wait(&status);
                }
            }
        }
        //Doesn't wait to finish
        else {
            if ((pid = fork()) == 0) {
                jrb_insert_int(pids, pid, JNULL);

                if (read != -1) dup2(read, 0);
                close(read);
                if (write != -1) dup2(write, 1);
                close(write);
                
                execvp(tempCmds[0], tempCmds);
                perror(tempCmds[0]);
                exit(1);
            }
        }
        lastWord = NULL;
        free(tempCmds);
        close(read);
        close(write);
        jrb_free_tree(pids);
    }

    jettison_inputstruct(is);
    return 0;
} 