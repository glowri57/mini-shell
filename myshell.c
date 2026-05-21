#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    char inp[1024];
    char *args[100];
   
    while (1) {
        printf("myshell> ");
        fflush(stdout);

        if(fgets(inp, sizeof(inp), stdin) == NULL) {
            break;
        }
        inp[strcspn(inp, "\n")] = '\0';
        if (inp[0] == '\0') {
            continue;
        }
        // tokenisation
        char *token = strtok(inp, " ");
         int i = 0;

        while(token != NULL) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;
        if(args[0] == NULL) {
            continue;
        }
        // exit built-in
       if (strcmp(args[0], "exit") == 0) {
        break;
       }
        // cd built-in    
       if (strcmp(args[0], "cd") == 0) {
        char *home = getenv("HOME");

        if (args[1] == NULL) {
            // cd → go to HOME
            if (home == NULL) {
                fprintf(stderr, "cd: HOME not set\n");
            } else {
                if (chdir(home) != 0) {
                    perror("cd");
                }
            }
        }
        else if (strcmp(args[1], "~") == 0) {
            // cd ~ → go to HOME
            if (home == NULL) {
                fprintf(stderr, "cd: HOME not set\n");
            } else {
                if (chdir(home) != 0) {
                    perror("cd");
                }
            }
        }
        else {
            // cd folder
            if (chdir(args[1]) != 0) {
                perror("cd");
            }
        }

        continue;
    }
        pid_t pid = fork();

        if(pid < 0) {
            perror("fork");
        }
        else if(pid == 0) {
            // char *args[] = {inp, NULL};
            execvp(args[0], args);
            perror("execvp");
            exit(1);
        }
        else  {
            waitpid(pid, NULL, 0);
        }
    }
}

