#define _DEFAULT_SOURCE

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

#define ITEM 10
#define BUFFER_SIZE 5

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t prod_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t cons_cond = PTHREAD_COND_INITIALIZER;

int BUFFER[BUFFER_SIZE];
int count = 0;
int head = 0;//Producer write position
int tail = 0;//Consumer read position

void* producer(void* arg){

    (void)arg;

    for(int i = 1 ; i <= ITEM ; i++){

        pthread_mutex_lock(&mutex);

        while(count == BUFFER_SIZE){

            printf("Buffer is full.\n");
            pthread_cond_wait(&prod_cond, &mutex);
        }

        BUFFER[head] = i;
        head = (head + 1) % BUFFER_SIZE;
        count++;
        printf("[Producer] Produced item: %d (Buffer count: %d)\n", i, count);


        pthread_cond_signal(&cons_cond);
        pthread_mutex_unlock(&mutex);

        usleep(100000);
    }

    pthread_exit(NULL);
}

void* consumer(void* arg){

    (void)arg;

    for(int i = 1 ; i <= ITEM ; i++){

        pthread_mutex_lock(&mutex);

        while(count == 0){
            printf("Buffer is empty.\n");
            pthread_cond_wait(&cons_cond, &mutex);
        }

        int item = BUFFER[tail];
        tail = (tail + 1) %  BUFFER_SIZE;
        count--;
        printf("[Consumer] Consumed item: %d (Buffer count: %d)\n", item, count);

        pthread_cond_signal(&prod_cond);
        pthread_mutex_unlock(&mutex);

        usleep(200000);
    }

    pthread_exit(NULL);
}


int main(){

    printf("=== Linux POSIX Condition Variable (Produer-Conusmer) demo.===\n");

    pthread_t prod;
    pthread_t cons;

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&prod_cond);
    pthread_cond_destroy(&cons_cond);

    printf("\n[Main] All tasks finished successfully.\n");

    return 0;
}