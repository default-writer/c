#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "std/common.h"

#include "playground/memory/api/v1/memory_v1.h"
#include "playground/memory/api/v2/memory_v2.h"
#include "playground/memory/api/v3/memory_v3.h"

struct memory_allocator_v1;
struct memory_allocator_v2;
struct memory_allocator_v3;

union memory_allocator_api {
    struct memory_allocator_v1 v1;
    struct memory_allocator_v2 v2;
    struct memory_allocator_v3 v3;
};

#endif /* _MEMORY_H_ */
