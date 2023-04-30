#ifndef _PLAYGROUND_MEMORY_V3_H_
#define _PLAYGROUND_MEMORY_V3_H_

#include "std/common.h"

struct memory_allocator_v3 {
    void (*init)(u64 size);
    void (*destroy)(void);
    void* (*alloc)(u64 size);
    void (*free)(void* data);
};

#endif /* _PLAYGROUND_MEMORY_V3_H_ */
