#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>

#define MAX_LINE 1024
#define MAX_ARGS 64

int main(){

    char line[MAX_LINE];
    char* args[MAX_ARGS];
    
    while(1){

        printf("myshell> ");
        fflush(stdout);

        if(fgets(line, sizeof(line), stdin) == NULL){
            printf("exiting myshell\n");
            break;
        }

        line[strcspn(line, "\n")] = '\0';

        if(strlen(line) == 0){
            continue;
        }

        int arg_count = 0;
        char* token = strtok(line, " ");
        while(token != NULL && arg_count < MAX_ARGS-1){
            args[arg_count] = token;
            arg_count++;
            token = strtok(NULL, " ");
        }
        args[arg_count] = NULL;

        if(strcmp(args[0], "exit") == 0){
            printf("Goodbye\n");
            break;
        }

        pid_t pid = fork();
        if(pid < 0){
            perror("fork failed\n");
        }
        else if(pid == 0){
            if(execvp(args[0], args) < 0){
                perror("myshell\n");
            }
            exit(EXIT_FAILURE);
        }
        else{

            int status;
            waitpid(pid, &status, 0);

        }


    }
    return 0;
}