#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main(void){

    printf("=== Linux Process Management Demo (PID : %d) === \n\n",getpid());

    pid_t pid = fork();
    
    if(pid < 0){

        perror("fork failed");
        return 1;
    }
    else if(pid == 0){

        printf("[Child]: I am the child process (PID: %d , parent PID:%d)\n",getpid(),getppid());
        printf("[Child]: Replacing the process image with 'ls -l' using execvp ... \n\n");

        char* args[] = {"ls","-l",NULL};
        execvp(args[0],args);

        perror("execvp failed");
        exit(1);
    }
    else{

        printf("[Parent]: I am the parent process ! Create the child process PID:%d\n",pid);
        printf("[Parent]: Waiting for the child process to complete ...\n");

        int status;

        waitpid(pid, &status, 0);

        if(WIFEXITED(status)){
            printf("\n[Parent]: Child exited normally with status : %d\n",WEXITSTATUS(status));
        }
        else{
            printf("\n[Parent]: Child terminated abnormally\n");
        }


    }

    return 0;
}