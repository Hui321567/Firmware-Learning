#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<stdint.h>

#define NUM_THREADS 2
#define COUNT_LINIT 10000 

static long counter = 0;
static pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;

void* count_up(void* arg){
    uintptr_t thread_id = (uintptr_t)arg;

    for(int i = 0 ; i < COUNT_LINIT ; i++){

        pthread_mutex_lock(&counter_mutex);

        counter++;

        pthread_mutex_unlock(&counter_mutex);
    }

    printf("[thread %lu] Finished counting\n",(unsigned long)thread_id);
    pthread_exit(NULL);
}

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

    for(int t = 0 ; t < NUM_THREADS ; t++){
        pthread_join(threads[t], NULL);
    }

    pthread_mutex_destroy(&counter_mutex);

    printf("\n[Main] All pthread_join finshed, final counter: %ld\n",counter);

    return 0;
}