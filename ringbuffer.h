#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <stdint.h>

#define BUFFER_SIZE_MAX 128U // Bytes.

typedef struct Ring_buffer_s {
    uint8_t len;
    void* head;
    void* tail;
    void* first;
    void* last;
    uint8_t datasz;
    uint8_t bufsz;
    uint32_t count;
} Ring_buffer_t;

int8_t Ring_buffer_init(Ring_buffer_t* buf, void* mem, uint8_t datasz,
                        uint8_t bufsz);

int8_t Ring_buffer_push(Ring_buffer_t* buffer, void* data);

int8_t Ring_buffer_pop(Ring_buffer_t* buf, void* data);

uint8_t Ring_buffer_is_full(Ring_buffer_t* buffer);

uint8_t Ring_buffer_is_empty(Ring_buffer_t* buffer);

#endif // RINGBUFFER_H
