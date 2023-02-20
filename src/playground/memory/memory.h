#ifndef _PLAYGROUND_MEMORY_H_
#define _PLAYGROUND_MEMORY_H_

#include "std/common.h"

struct memory_allocator {
    void (*init)(void);
    void (*destroy)(void);
    void* (*alloc)(u64 size);
    void (*free)(const void* data, u64 size);
};

#endif // _PLAYGROUND_MEMORY_H_
