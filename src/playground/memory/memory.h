#ifndef _PLAYGROUND_MEMORY_H_
#define _PLAYGROUND_MEMORY_H_

#include "std/common.h"

struct memory_allocator {
    void (*init)();
    void (*destroy)();
    void* (*alloc)(u32 size);
    void (*free)(const void* data, u32 size);
};

#endif // _PLAYGROUND_MEMORY_H_