#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

#define NUM_THREADS 2
#define COUNT_LINIT 10000 

int main(){

    printf("=== Linux POSIX Threads (pthread) & Mutex demo ===\n");
    printf("Expected final counter value : %d\n\n", NUM_THREADS * COUNT_LINIT);

    pthread_t threads[NUM_THREADS];
    int rc;

    for(uintptr_t t = 0 ; t < NUM_THREADS ; t++ ){
        rc = pthread_create(&threads[t], NULL, count_up, (void*)t);
        if(rc < 0){
            printf("Error: pthread_create failed with code: %d\n",rc);
            exit(-1);
        }
    }
}