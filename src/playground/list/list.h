#ifndef _PLAYGROUND_LIST_H_
#define _PLAYGROUND_LIST_H_

#include "std/common.h"

struct memory_allocator {
    void (*init)();
    void (*destroy)();
    void* (*alloc)(u32 nmemb, u32 size);
    void (*free)(void* data, u32 size);
};

#endif // _PLAYGROUND_LIST_H_