#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    
    if (argc < 2) {
        write(STDERR_FILENO,"Usage: ./io_test <filename>\n",27);
        return 1;
    }

    int fd = open(argv[1],O_RDONLY);
    if(fd < 0){
        perror("Error file opening");
        return 1;
    }
    
    char BUFFER[BUFFER_SIZE];
    ssize_t bytes_read;

    while((bytes_read = read(fd,BUFFER,BUFFER_SIZE)) > 0){
        ssize_t bytes_written = write(STDOUT_FILENO,BUFFER,bytes_read);
        if(bytes_written < 0){
            perror("Error writting to stdout");
            close(fd);
            return 1;
        }
    }

    if(bytes_read < 0){
        perror("Error reading file");
        close(fd);
        return 1;
        
    }

    close(fd);
    return 0;
}