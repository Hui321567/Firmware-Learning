#include<stdio.h>
#include "ring_buffer.h"

void on_data_received(int data){
    printf("[UART Callback] Processed Byte: 0x%02X (%d)\n", data, data);
}

int main(){

    ring_buffer_t rb;
    ring_buffer_init(&rb);

    printf("=== Pure C ring_buffer & Function Pointer test ===\n");

    printf("---TEST1 EMPTY POP---\n");
    int data;
    if(ring_buffer_pop(&rb, &data)){
        printf("TEST1 FAIL: pop from the empty buffer sould be failed\n");
    }
    else{
        printf("TEST1 PASS: pop from the empty buffer correctly failed\n");
    }

    printf("---TEST2 FILL BUFFER---\n");
    for(int i = 0 ; i < RING_BUFFER_SIZE ; i++){
        if(!ring_buffer_push(&rb, i)){
            printf("TEST2 FAIL: %dth push failed\n", i);
        }
    }
    if(ring_buffer_isFull(&rb)){
        printf("TEST2 PASS: buffer is full\n");
    }
    else{
        printf("TEST2 FAIL: buffer sould be full\n");
    }

    printf("---TEST3 FULL PUSH---\n");
    if(ring_buffer_push(&rb, 100)){
        printf("TEST3 FAIL: push to full buffer sould be failed\n");
    }
    else{
        printf("TEST3 PASS: push to full buffer correctly failed\n");
    }

    printf("---TEST4 WARP-AROUND---\n");
    for(int i = 0 ; i < 3 ; i++){
        if(!ring_buffer_pop(&rb, &data)){
            printf("POP FAILED\n");
        }
    }

    if(!ring_buffer_push(&rb, 100) || !ring_buffer_push(&rb, 101) || !ring_buffer_push(&rb, 102)){
        printf("TEST4 FAIL: warp-around push failed\n");
    }
    else{
        printf("TEST4 PASS: warp-around push succeeded\n");
    }

    printf("---TEST5 FIFO ORDER---\n");

    int expect_data[] = {3, 4, 5, 6, 7, 100, 101, 102};
    for(int i = 0 ; i < RING_BUFFER_SIZE ; i++){
        if(!ring_buffer_pop(&rb, &data)){
            printf("TEST5 FAIL: pop failed\n");
        }
        else{
            if(data != expect_data[i]){
                printf("TEST5 FAIL: expect %d but got %d\n", expect_data[i], data);
            }
        }
    }
    printf("TEST5 PASS: FIFO order is correct\n");

    printf("---TEST6 NULL DATA POINTER CHECK---\n");
    if(ring_buffer_pop(&rb, NULL)){
        printf("TEST6 FAIL: pop with NULL data pointer solud be failed\n");
    }
    else{
        printf("TEST6 PASS: pop with NULL data pointer correctly failed\n");
    }
    //printf("Pushing elements: 10, 20, 30, 40, ...\n");

    //for(int i = 1 ; i <= RING_BUFFER_SIZE ; i++){
        //ring_buffer_push(&rb, i*10);
    //}

    //printf("--- Processing Buffer via Function Pointer Callback ---\n");
    //ring_buffer_process(&rb, on_data_received);

    //if(ring_buffer_isEmpty(&rb)){
        //printf("Ring buffer now is empty. All callbacks executed successfully!\n");
    //}

    return 0;
}