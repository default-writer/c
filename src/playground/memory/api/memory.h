#ifndef _PLAYGROUND_MEMORY_H_
#define _PLAYGROUND_MEMORY_H_

#include "std/common.h"

#include "playground/memory/api/v1/memory.h"
#include "playground/memory/api/v2/memory.h"
#include "playground/memory/api/v3/memory.h"

struct memory_allocator_v1;
struct memory_allocator_v2;
struct memory_allocator_v3;

union memory_allocator_api {
    struct memory_allocator_v1 v1;
    struct memory_allocator_v2 v2;
    struct memory_allocator_v3 v3;
};

#endif /* _PLAYGROUND_MEMORY_H_ */
