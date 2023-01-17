#ifndef _COMMON_MEMORY_H_
#define _COMMON_MEMORY_H_

#include "std/common.h"

struct data;

struct memory_allocator_data {
    /* private data definition */
    struct data* data;
};

struct memory_allocator {
    void (*init)();
    void (*destroy)();
    void* (*alloc)(u32 nmemb, u32 size);
    void (*free)(void* data, u32 size);
};

#endif // _COMMON_MEMORY_H_