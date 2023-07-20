#ifndef _POINTER_TYPES_DATA_H_
#define _POINTER_TYPES_DATA_H_

#include "std/common.h"

struct data_methods {
    u64 (*alloc)(u64 size);
    void (*free)(u64 ptr);
    void* (*unsafe)(u64 ptr);
    u64 (*size)(u64 ptr);
};

#endif /* _POINTER_TYPES_DATA_H_ */