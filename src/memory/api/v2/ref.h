#ifndef _MEMORY_REF_V2_H_
#define _MEMORY_REF_V2_H_

#include "std/common.h"

#include "memory/api/ref.h"

struct memory_ref_methods {
    void (*init)(u64 size);
    void (*destroy)(void);
    void* (*alloc)(u64 size);
    void (*free)(void* data);
    void* (*pop)(void);
    void* (*peek)(void);
};

#endif // _MEMORY_REF_V2_H_
