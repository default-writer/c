#ifndef _MEMORY_REF_H_
#define _MEMORY_REF_H_

#include "std/common.h"

struct memory_ref {
    struct memory_ref* prev;
    struct memory_ref* next;
    u64 size;
};

struct memory_ref_methods {
    struct memory_ref* (*ref)(void* data);
    void* (*ptr)(void* data);
    u64 (*size)(void* data);
    void* (*alloc)(u64 size);
    void (*free)(void* ptr);
};

#endif // _MEMORY_REF_H_
