#ifndef _POINTER_TYPES_VIRTUAL_H_
#define _POINTER_TYPES_VIRTUAL_H_

#include "std/common.h"

struct virtual_methods {
    u64 (*alloc)(u64 size);
    void (*free)(u64 ptr);
    void* (*unsafe)(u64 ptr);
};

#endif /* _POINTER_TYPES_VIRTUAL_H_ */