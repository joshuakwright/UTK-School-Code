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

char**
CopyList(char **list, int size) {
    char **newList = malloc(size*sizeof(char*));
    for (int i = 0; i < size; i++) newList[i] = list[i];
    return newList;
}

int main(int argc, char** argv) {
    IS is;
    is = new_inputstruct(NULL);
    char **tempCmd = NULL;

    while(get_line(is) >= 0) {
        int pipes = 0;
        JRB pids = make_jrb();
        Dllist commands = new_dllist();
        int size = is->NF+1;
        tempCmd = malloc(size*sizeof(char*));
        for (int i = 0, j = 0; j < is->NF; i++, j++) {
            if (strcmp(is->fields[j], "|") == 0) {
                tempCmd[i] = (char*)NULL;
                i = -1;
                pipes++;
                dll_append(commands, new_jval_v(CopyList(tempCmd, size)));
                free(tempCmd);
                tempCmd = malloc(size*sizeof(char*));
            }
            else if(j + 1 == is->NF) {
                tempCmd[i] = strdup(is->fields[j]);
                tempCmd[i+1] = (char*)NULL;
                dll_append(commands, new_jval_v(tempCmd));
            }
            else {
                tempCmd[i] = strdup(is->fields[j]);
            }
        }

        char **command = NULL;
        int fd1, fd2, fd3;
        char noWait = ' ';
        int status;
        Dllist temp;
        int fd[2 * pipes]; 

        for(int i = 0; i < pipes; i++) {
            if(pipe(fd + i*2) < 0) {
                perror("pipe");
                exit(1);
            }
        }
        
        int tracker = 0;
        dll_traverse(temp, commands) {
            int pid;
            char *place = "";
            if (temp == dll_last(commands) && temp != dll_first(commands)) place = "last";
            else if ((temp != dll_first(commands) && temp != dll_last(commands)) || (temp == dll_first(commands) && temp != dll_last(commands))) place = "middle";

            char **tempCmds = (char**)jval_v(temp->val);
            for(int i = 0; tempCmds[i] != NULL; i++) {
                if(strcmp(tempCmds[i], "&") == 0) noWait = '&';
            }

            if ((pid = fork()) == 0) {
                free(command);
                command = malloc(size*sizeof(char*));

                if (strcmp(place, "middle") == 0) {
                    if(dup2(fd[tracker + 1], 1) != 1) {
                        perror("dup2");
                        exit(1);
                    }
                }
                if(tracker != 0) {
                    if(dup2(fd[tracker-2], 0) != 0) {
                        perror("dup2");
                        exit(1);
                    }
                }

                for (int i = 0; i < 2 * pipes; i++) {
                    close(fd[i]);
                }
                
                for(int i = 0; tempCmds[i] != NULL; i++) {
                    if (strcmp(tempCmds[i], "&") == 0) command[i] = NULL;
                    else if (strcmp(tempCmds[i], "<") == 0) {
                        command[i] = NULL;
                        fd1 = open(tempCmds[++i], O_RDONLY);
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
                    else if (strcmp(tempCmds[i], ">") == 0) {
                        command[i] = (char *)0;
                        fd2 = open(tempCmds[++i], O_WRONLY | O_TRUNC | O_CREAT, 0644);
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
                    else if (strcmp(tempCmds[i], ">>") == 0) {
                        command[i] = NULL;
                        fd3 = open(tempCmds[++i], O_WRONLY | O_APPEND, 0644);
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
                    else command[i] = tempCmds[i];
                }
                command[size-1] = (char*)NULL;
                execvp(command[0], command);
                perror(command[0]);
                exit(1);
            }
            else {
                jrb_insert_int(pids, pid, JNULL);
            } 
            tracker += 2;
        }
        
        for (int i = 0; i < 2 * pipes; i++) {
            close(fd[i]);
        }
        
        while(!jrb_empty(pids)) {
            if (noWait != '&') {
                int currPid = wait(&status);
                if (jrb_find_int(pids, currPid) != NULL) {
                    jrb_delete_node(jrb_find_int(pids, currPid));
                }
                if (getpid() != status) {
                    currPid = wait(&status);
                    if (jrb_find_int(pids, currPid) != NULL) {
                        jrb_delete_node(jrb_find_int(pids, currPid));
                    }
                }
                else {break;}
            }
            else {
                break;
            }
        }
        noWait = ' ';

        dll_traverse(temp, commands) {
            char **tempCmds = (char**)jval_v(temp->val);
            for(int i = 0; tempCmds[i] != NULL; i++) {
            //    free(tempCmds[i]);
            }
            free(tempCmds);
        }
        free_dllist(commands);
        jrb_free_tree(pids);
    }

    jettison_inputstruct(is);
    return 0;
} 


/*

char **command = NULL;
        int fd1 = -1, fd2 = -1, fd3 = -1;
        char noWait = ' ';
        int status;
        Dllist temp;
        int fd[2]; 
        int prev_fd = -1;
        int error;
        int pid;
        
        dll_traverse(temp, commands) {
            char **tempCmds = (char**)jval_v(temp->val);
            for(int i = 0; tempCmds[i] != NULL; i++) {
                if(strcmp(tempCmds[i], "&") == 0) noWait = '&';
            }

            char *place = "none";
            if (temp == dll_first(commands) && temp != dll_last(commands)) place = "first";
            if (temp == dll_last(commands) && temp != dll_first(commands)) place = "last";
            else if ((temp != dll_first(commands) && temp != dll_last(commands))) place = "middle";

            if (strcmp(place, "none") != 0) {
                error = pipe(fd);
                if (error < 0) {
                    perror("pipe");
                    exit(1);
                }
            }
            
            if ((pid = fork()) == 0) {
                free(command);
                command = malloc(size*sizeof(char *));
                for(int i = 0; tempCmds[i] != NULL; i++) {
                    if (strcmp(tempCmds[i], "&") == 0) command[i] = NULL;
                    else if (strcmp(tempCmds[i], "<") == 0) {
                        command[i] = NULL;
                        fd1 = open(tempCmds[++i], O_RDONLY);
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
                    else if (strcmp(tempCmds[i], ">") == 0) {
                        command[i] = (char *)0;
                        fd2 = open(tempCmds[++i], O_WRONLY | O_TRUNC | O_CREAT, 0644);
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
                    else if (strcmp(tempCmds[i], ">>") == 0) {
                        command[i] = NULL;
                        fd3 = open(tempCmds[++i], O_WRONLY | O_APPEND, 0644);
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
                    else command[i] = tempCmds[i];
                }

                if(strcmp(place, "first") == 0) {
                    if (dup2(fd[1], 1) != 1) {
                        perror("dup2(pipefd[1]) line123");
                        exit(1);
                    }
                    close(fd[0]);
                }
                else if (strcmp(place, "middle") == 0) {
                    if (dup2(prev_fd, 0) != 0) {   
                        perror("dup2(pipefd[0]) line130");
                        exit(1);
                    }
                    if (dup2(fd[1], 1) != 1) {
                        perror("dup2(pipefd[1]) line 134");
                        exit(1);
                    }
                }
                else if(strcmp(place, "last") == 0) {
                    if (dup2(prev_fd, 0) != 0) {   
                        perror("dup2(pipefd[0]) line140");
                        exit(1);
                    }
                    close(fd[0]);
                    close(fd[1]);
                }
                command[size-1] = (char*)NULL;
                execvp(command[0], command);
                perror(command[0]);
                exit(1);
            }
            else {
                close(prev_fd);
                prev_fd = fd[0];
                close(fd[1]);
                jrb_insert_int(pids, pid, JNULL);
            } 
        }
        
        while(!jrb_empty(pids)) {
            if (noWait != '&') {
                int currPid = wait(&status);
                if (jrb_find_int(pids, currPid) != NULL) {
                    jrb_delete_node(jrb_find_int(pids, currPid));
                }
                if (getpid() != status) {
                    currPid = wait(&status);
                    if (jrb_find_int(pids, currPid) != NULL) {
                        jrb_delete_node(jrb_find_int(pids, currPid));
                    }
                }
                else {break;}
            }
            else {
                break;
            }
        }
        noWait = ' ';

        dll_traverse(temp, commands) {
            char **tempCmds = (char**)jval_v(temp->val);
            for(int i = 0; tempCmds[i] != NULL; i++) {
            //    free(tempCmds[i]);
            }
            free(tempCmds);
        }
        free_dllist(commands);
        jrb_free_tree(pids);
    }

*/



/*

char **command = NULL;
        int fd1, fd2, fd3;
        char noWait = ' ';
        int status;
        Dllist temp;
        printf("%i\n\n", pipes);
        int fd[2 * pipes]; 

        for(int i = 0; i < pipes; i++) {
            if(pipe(fd + i*2) < 0) {
                perror("pipe");
                exit(1);
            }
        }
        
        int tracker = 0;
        dll_traverse(temp, commands) {
            int pid;
            char *place = "";
            //if (temp == dll_first(commands) && temp != dll_last(commands)) place = "first";
            if (temp == dll_last(commands) && temp != dll_first(commands)) place = "last";
            else if ((temp != dll_first(commands) && temp != dll_last(commands)) || (temp == dll_first(commands) && temp != dll_last(commands))) place = "middle";

            if(strcmp(place, "first") == 0) {
                error = pipe(fd);
                if (error < 0) {
                    perror("pipe");
                    exit(1);
                }
                if (dup2(fd[1], 1) != 1) {
                        perror("dup2(pipefd[1])");
                        exit(1);
                }
                close(fd[1]);
                close(fd[0]);
                //close(fd[0]);
                //readFd = fd[0];
            }

            char **tempCmds = (char**)jval_v(temp->val);
            for(int i = 0; tempCmds[i] != NULL; i++) {
                if(strcmp(tempCmds[i], "&") == 0) noWait = '&';
            }

            if ((pid = fork()) == 0) {
                free(command);
                command = malloc(size);

                if (strcmp(place, "middle") == 0) {
                    if(dup2(fd[tracker + 1], 1) != 1) {
                        perror("dup2");
                        exit(1);
                    }
                    
                    //close(fd[1]);
                    //fd[0] = readFd;
                    error = pipe(fd);
                    if (error < 0) {
                        perror("pipe");
                        exit(1);
                    }
                    if (dup2(fd[0], 0) != 0) {   
                        perror("dup2(pipefd[0])");
                        exit(1);
                    }
                    //close(fd[0]);
                    if (dup2(fd[1], 1) != 1) {
                        perror("dup2(pipefd[1])");
                        exit(1);
                    }
                    close(fd[1]);
                    close(fd[0]);
                    //readFd = fd[0];
                }
                if(tracker != 0) {//strcmp(place, "last") == 0) {
                    if(dup2(fd[tracker-2], 0) != 0) {
                        perror("dup2");
                        exit(1);
                    }
                    
                    //close(fd[1]);
                    error = pipe(fd);
                    if (error < 0) {
                        perror("pipe");
                        exit(1);
                    }
                    if (dup2(fd[0], 0) != 0) {   
                        perror("dup2(pipefd[0])");
                        exit(1);
                    }
                    close(fd[1]);
                    close(fd[0]);
                }

                for (int i = 0; i < 2 * pipes; i++) {
                    close(fd[i]);
                }
                
                for(int i = 0; tempCmds[i] != NULL; i++) {
                    if (strcmp(tempCmds[i], "&") == 0) command[i] = NULL;
                    else if (strcmp(tempCmds[i], "<") == 0) {
                        command[i] = NULL;
                        fd1 = open(tempCmds[++i], O_RDONLY);
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
                    else if (strcmp(tempCmds[i], ">") == 0) {
                        command[i] = (char *)0;
                        fd2 = open(tempCmds[++i], O_WRONLY | O_TRUNC | O_CREAT, 0644);
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
                    else if (strcmp(tempCmds[i], ">>") == 0) {
                        command[i] = NULL;
                        fd3 = open(tempCmds[++i], O_WRONLY | O_APPEND, 0644);
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
                    else command[i] = tempCmds[i];
                }
                command[size-1] = (char*)NULL;
                execvp(command[0], command);
                perror(command[0]);
                exit(1);
            }
            else {
                jrb_insert_int(pids, pid, JNULL);
            } 
            tracker += 2;
        }
        
        for (int i = 0; i < 2 * pipes; i++) {
            close(fd[i]);
        }
        
        while(!jrb_empty(pids)) {
            if (noWait != '&') {
                int currPid = wait(&status);
                if (jrb_find_int(pids, currPid) != NULL) {
                    jrb_delete_node(jrb_find_int(pids, currPid));
                }
                if (getpid() != status) {
                    currPid = wait(&status);
                    if (jrb_find_int(pids, currPid) != NULL) {
                        jrb_delete_node(jrb_find_int(pids, currPid));
                    }
                }
                else {break;}
            }
            else {
                break;
            }
        }
        noWait = ' ';

        dll_traverse(temp, commands) {
            char **tempCmds = (char**)jval_v(temp->val);
            for(int i = 0; tempCmds[i] != NULL; i++) {
            //    free(tempCmds[i]);
            }
            free(tempCmds);
        }
        free_dllist(commands);
        jrb_free_tree(pids);
    }

*/