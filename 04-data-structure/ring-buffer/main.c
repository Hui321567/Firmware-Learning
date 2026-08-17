#include<stdio.h>
#include "ring_buffer.h"

void on_data_received(int data){
    printf("[UART Callback] Processed Byte: 0x%02X (%d)\n", data, data);
}

int main(){

    ring_buffer_t rb;
    ring_buffer_init(&rb);

    printf("=== Pure C ring_buffer & Function Pointer test ===\n");

    printf("Pushing elements: 10, 20, 30, 40, ...\n");

    for(int i = 1 ; i <= 4 ; i++){
        ring_buffer_push(&rb, i*10);
    }

    printf("--- Processing Buffer via Function Pointer Callback ---\n");
    ring_buffer_process(&rb, on_data_received);

    if(ring_buffer_isEmpty(&rb)){
        printf("Ring buffer now is empty. All callbacks executed successfully!\n");
    }

    return 0;
}