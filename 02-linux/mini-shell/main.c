#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>

#define MAX_LINE 1024
#define MAX_ARGS 64

void parse_args(char* line, char** args){
    int arg_count = 0;
        char* token = strtok(line, " ");
        while(token != NULL && arg_count < MAX_ARGS-1){
            args[arg_count] = token;
            arg_count++;
            token = strtok(NULL, " ");
        }
        args[arg_count] = NULL;
}

void execute_pipe(char* left_cmd, char* right_cmd){
    char* left_args[MAX_ARGS];
    char* right_args[MAX_ARGS];

    parse_args(left_cmd, left_args);
    parse_args(right_cmd, right_args);

    int pipefd[2];
    if(pipe(pipefd) == -1){
        perror("pipe failed");
        return;
    }
    
    pid_t pid1 = fork();

    if(pid1 == -1){
        perror("pid1 fork failed");
        close(pipefd[0]);
        close(pipefd[1]);
        return;
    }
    else if(pid1 == 0){
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        if(execvp(left_args[0],left_args) == -1){
            perror("left cmd failed");
            exit(EXIT_FAILURE);
        }
    }

    pid_t pid2 = fork();

    if(pid2 == -1){
        perror("pid2 fork failed");
        close(pipefd[0]);
        close(pipefd[1]);
        waitpid(pid1, NULL, 0);
        return;
    }
    else if(pid2 == 0){
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);

        if(execvp(right_args[0],right_args) == -1){
            perror("right cmd failed");
            exit(EXIT_FAILURE);
        }
    }

    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

}

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

        char* pipe_pos = strchr(line, '|');
        if(pipe_pos != NULL){
            *pipe_pos = '\0';
            char* left_cmd = line;
            char* right_cmd = pipe_pos + 1;
            execute_pipe(left_cmd, right_cmd);
            continue;
        }

        


        parse_args(line, args);


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