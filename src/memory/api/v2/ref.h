#ifndef _MEMORY_REF_V2_H_
#define _MEMORY_REF_V2_H_

#include "std/common.h"

#include "memory/api/ref.h"

struct memory_ref_methods {
    void (*init)(u64 size);
    void (*destroy)(void);
    // struct memory_ref* (*ref)(void* data);
    // void* (*ptr)(struct memory_ref* data);
    u64 (*size)(void* data);
    void* (*alloc)(u64 size);
    void (*free)(void* ptr);
    void (*push)(void* data);
    void* (*pop)(void);
    // void (*use)(void* data);
    void* (*peek)(void);
};

#endif // _MEMORY_REF_V2_H_
