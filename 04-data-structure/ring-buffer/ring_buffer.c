#include"ring_buffer.h"

void ring_buffer_init(ring_buffer_t* rb){
    if(rb == NULL) return;
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
}

bool ring_buffer_isFull(const ring_buffer_t* rb){
    return rb ? (rb->count == RING_BUFFER_SIZE):false;
}

bool ring_buffer_isEmpty(const ring_buffer_t* rb){
    return rb ? (rb->count == 0):false;
}

bool ring_buffer_push(ring_buffer_t* rb,int data){
    if(!rb || ring_buffer_isFull(rb)) return false;
    rb->buffer[rb->head] = data;
    rb->head = (rb->head + 1) % RING_BUFFER_SIZE;
    rb->count++;
    return true;
}

bool ring_buffer_pop(ring_buffer_t* rb,int* data){
    if(!rb || ring_buffer_isEmpty(rb) || !data) return false;
    *data = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1) % RING_BUFFER_SIZE;
    rb->count--;
    return true;
}

void ring_buffer_process(ring_buffer_t* rb, ring_buffer_callback_t callback){
    if(!rb || !callback) return;

    int data;
    while(ring_buffer_pop(rb, &data)){
        callback(data);
    }
}