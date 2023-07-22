#ifndef _POINTER_TYPES_OBJECT_H_
#define _POINTER_TYPES_OBJECT_H_

#include "std/common.h"

struct object_methods {
    u64 (*alloc)(u64 size);
    u64 (*free)(u64 ptr);
    u64 (*load)(const void* data, u64 size);
    void* (*unsafe)(u64 ptr);
    u64 (*size)(u64 ptr);
};

#endif /* _POINTER_TYPES_OBJECT_H_ */