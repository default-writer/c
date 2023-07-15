#ifndef _POINTER_TYPES_OBJECT_H_
#define _POINTER_TYPES_OBJECT_H_

#include "std/common.h"

struct object_methods {
    u64 (*alloc)(void);
    u64 (*load)(const char* data);
    char* (*unsafe)(u64 ptr);
    void (*free)(u64 ptr);
};

#endif /* _POINTER_TYPES_OBJECT_H_ */