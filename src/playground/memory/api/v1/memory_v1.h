#ifndef _MEMORY_API_V1_H_
#define _MEMORY_API_V1_H_

#include "std/common.h"

struct memory_allocator_v1 {
    void (*init)(void);
    void (*destroy)(void);
    void* (*alloc)(u64 size);
    void (*free)(void* data, u64 size);
};

#endif /* _MEMORY_API_V1_H_ */
