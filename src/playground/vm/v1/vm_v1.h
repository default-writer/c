#ifndef _VIRTUAL_H_
#define _VIRTUAL_H_

#include "std/common.h"

struct vm_data;

struct vm {
    void (*init)(u64 size);
    void (*destroy)(void);
    struct pointer* (*free)(u64 address);
    struct pointer* (*read)(u64 address);
    u64 (*write)(struct pointer* value);
#ifdef USE_MEMORY_DEBUG_INFO
    void (*dump)(void);
    void (*dump_ref)(void);
#endif
};

struct vm_type {
    void (*free)(u64 args);
};

#endif /* _VIRTUAL_H_ */
