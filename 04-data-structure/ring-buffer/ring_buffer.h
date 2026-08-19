#ifndef RING_BUFFER_H
#define RING_BUFFER_H
#define RING_BUFFER_SIZE 8

#include<stdbool.h>
#include<stddef.h>

typedef struct {
    int buffer[RING_BUFFER_SIZE];
    size_t tail;
    size_t head;
    size_t count;
}ring_buffer_t;

typedef void (*ring_buffer_callback_t)(int data);

void ring_buffer_init(ring_buffer_t* rb);
bool ring_buffer_isFull(const ring_buffer_t* rb);
bool ring_buffer_isEmpty(const ring_buffer_t* rb);
bool ring_buffer_push(ring_buffer_t* rb,int data);
bool ring_buffer_pop(ring_buffer_t* rb,int* data);

void ring_buffer_process(ring_buffer_t* rb, ring_buffer_callback_t callback);

#endif