#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

#define BUFFER 100

int main(void){

    int pipefd[2];
    pid_t pid;
    
    char write_msg[] = "Hello Child ! Message through the IPC pipe";
    char read_msg[BUFFER];

    if(pipe(pipefd) == -1){
        perror("pipefd creation failed !");
        return 1;
    }

    pid = fork();

    if(pid < 0){
        perror("fork failed");
        return 1;
    }
    else if(pid == 0){

        close(pipefd[1]);

        ssize_t bytes_read = read(pipefd[0], read_msg, sizeof(read_msg)-1);
        if(bytes_read > 0){
            read_msg[bytes_read] = '\0';
            printf("[Child]: Received message from Parent: \"%s\" \n",read_msg);
        }

        close(pipefd[0]);
        exit(0);
    }
    else{
        close(pipefd[0]);

        printf("[Parent]: Sending message to Child through pipe ...\n");

        write(pipefd[1], write_msg, strlen(write_msg));

        close(pipefd[1]);

        wait(NULL);
        printf("[Parent]: Child finished processing. Pipe demo completed successfully.\n");
    }

    return 0;

}