#ifndef _PLAYGROUND_MEMORY_V2_H_
#define _PLAYGROUND_MEMORY_V2_H_

#include "std/common.h"

struct memory_allocator {
    void (*init)(u64 size);
    void (*destroy)(void);
    void* (*alloc)(u64 size);
    void (*free)(void* data);
};

#endif // _PLAYGROUND_MEMORY_V2_H_
