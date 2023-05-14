#ifndef _MEMORY_API_V2_H_
#define _MEMORY_API_V2_H_

#include "std/common.h"

struct memory_allocator_v2 {
    void (*init)(void);
    void (*destroy)(void);
    void* (*alloc)(u64 size);
    void (*free)(void* data);
};

#endif /* _MEMORY_API_V2_H_ */
