#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include "fields.h"

int main(int argc, char** argv) {
    IS is;
    is = new_inputstruct(NULL);
    char **command = NULL;
    int fd1, fd2, fd3;
    
    while(get_line(is) >= 0) {
        int status;
        if (fork() == 0) {
            int size = is->NF+1;
            free(command);
            command = malloc(size);

            //loops through fields array and creates the command for execvp or dups the needed files
            for (int i = 0; i < is->NF; i++) {
                if (strcmp(is->fields[i], "&") == 0) command[i] = NULL;
                //stdin
                else if (strcmp(is->fields[i], "<") == 0) {
                    command[i] = NULL;
                    fd1 = open(is->fields[++i], O_RDONLY);
                    command[i] = NULL;
                    if (fd1 < 0) {
                        perror("open\n");
                        exit(1);
                    }
                    if (dup2(fd1, 0) != 0) {
                        perror("dup\n");
                        exit(1);
                    }
                    close(fd1);
                }
                //stdout
                else if (strcmp(is->fields[i], ">") == 0) {
                    command[i] = (char *)0;
                    fd2 = open(is->fields[++i], O_WRONLY | O_TRUNC | O_CREAT, 0644);
                    command[i] = NULL;
                    if (fd2 < 0) {
                        perror("open\n");
                        exit(1);
                    }
                    if (dup2(fd2, 1) != 1) {
                        perror("dup\n");
                        exit(1);
                    }
                    close(fd2);
                }
                //append
                else if (strcmp(is->fields[i], ">>") == 0) {
                    command[i] = NULL;
                    fd3 = open(is->fields[++i], O_WRONLY | O_APPEND, 0644);
                    command[i] = NULL;
                    if (fd3 < 0) {
                        perror("open\n");
                        exit(1);
                    }
                    if (dup2(fd3, 1) != 1) {
                        perror("dup\n");
                        exit(1);
                    }
                    close(fd3);
                }
                else command[i] = is->fields[i];
            }
            command[size-1] = (char*)NULL;
            status = execvp(command[0], command);
            perror(command[0]);
            exit(1);
        }
        else {
            if (strcmp(is->fields[is->NF-1], "&") != 0) {
                wait(&status);
                if (getpid() != status) wait(&status);
            }
        }
    }

    jettison_inputstruct(is);
    return 0;
}