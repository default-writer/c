#ifndef _MEMORY_API_REF_V2_H_
#define _MEMORY_API_REF_V2_H_

#include "memory/api/common/ref.h"
#include "std/common.h"

struct memory_ref_methods {
    void (*init)(u64 size);
    void (*destroy)(void);
    u64 (*size)(void* data);
    void* (*alloc)(u64 size);
    void (*free)(void* data);
    void (*push)(void* data);
    void* (*pop)(void);
    void* (*peek)(void);
};

#endif /* _MEMORY_API_REF_V2_H_ */
