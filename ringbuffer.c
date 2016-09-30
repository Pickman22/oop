#include <stddef.h>
#include <string.h>
#include "ringbuffer.h"


int8_t Ring_buffer_init(Ring_buffer_t* buf, void* mem, uint8_t datasz,
                        uint8_t bufsz)
{
    int8_t ret = -1;
    if(buf && mem && (datasz > 0U) && (bufsz > 0U) &&
      ((bufsz * datasz) <= BUFFER_SIZE_MAX) && (datasz <= bufsz))
    {
        ret = 0;
        buf->head = mem;
        buf->tail = mem;
        buf->datasz = datasz;
        buf->bufsz = bufsz;
        buf->first = mem;
        buf->last = (mem + (bufsz - 1U) * datasz);
        buf->count = 0U;
    }
    return ret;
}

int8_t Ring_buffer_push(Ring_buffer_t* buf, void* data) {
    int8_t ret = -1;
    if(data && buf) {
        ret = 0;
        memcpy(buf->head, data, buf->datasz);
        /* Update data count. */
        buf->count = (buf->count == buf->bufsz) ? buf->count : buf->count + 1U;

        /* Update pointers. */
        buf->head = (buf->head == buf->last) ? buf->first :
                                               buf->head + buf->datasz;
        /* When the buffer is full, the tail ptr tracks the head ptr. The
        head ptr can't pass the tail pointer. */
        if(buf->count == buf->bufsz) {
            buf->tail = buf->head;
        }
    }
    return ret;
}

int8_t Ring_buffer_pop(Ring_buffer_t* buf, void* data) {
    int ret = -1;
    if(data && buf) {
        if(buf->count > 0U) {
            ret = 0;
            memcpy(data, buf->tail, buf->datasz);
            /* Update data count. */
            buf->count = (buf->count == 0U) ? 0U : buf->count - 1U;

            /* Update pointers. */
            buf->tail = (buf->tail == buf->last) ? buf->first :
                                                   buf->tail + buf->datasz;
        }
    }
    return ret;
}

uint8_t Ring_buffer_is_full(Ring_buffer_t* buf) {
    uint8_t ret = 0U;
    if(buf) {
        ret = (buf->count == buf->bufsz);
    }
    return ret;
}

uint8_t Ring_buffer_is_empty(Ring_buffer_t* buf) {
    uint8_t ret = 0U;
    if(buf) {
        ret = (buf->count == 0U);
    }
    return ret;
}
