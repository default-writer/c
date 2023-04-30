#ifndef _MEMORY_REF_V1_H_
#define _MEMORY_REF_V1_H_

#include "memory/api/ref.h"
#include "std/common.h"

struct memory_ref_methods {
    void (*init)(void);
    void (*destroy)(void);
    u64 (*size)(void* data);
    void* (*alloc)(u64 size);
    void (*free)(void* ptr);
    void (*push)(void* data);
    void* (*pop)(void);
    void* (*peek)(void);
};

#endif /* _MEMORY_REF_V1_H_ */
