#ifndef _PLAYGROUND_MEMORY_V5_H_
#define _PLAYGROUND_MEMORY_V5_H_

#include "std/common.h"

struct memory_allocator {
    void (*init)(void);
    void (*destroy)(void);
    void* (*alloc)(u64 size);
    void (*free)(void* data);
};

#endif // _PLAYGROUND_MEMORY_V5_H_
