#ifndef _MEMORY_REF_V3_H_
#define _MEMORY_REF_V3_H_

#include "std/common.h"

struct memory_ref {
    struct memory_ref* cache;
    struct memory_ref* prev;
    struct memory_ref* next;
    u64 address_space;
    u64 size;
};

struct memory_ref_methods {
    void (*init)(u64 size);
    void (*destroy)(void);
    void* (*alloc)(u64 size);
    void (*free)(void* data);
    void* (*pop)(void);
    void* (*peek)(void);
};

#endif // _MEMORY_REF_V3_H_
