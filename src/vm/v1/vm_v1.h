#ifndef _VIRTUAL_H_
#define _VIRTUAL_H_

#include "std/common.h"

struct vm;
struct vm_data;

struct vm_methods {
    struct vm* (*init)(u64 size);
    void (*destroy)(struct vm**);
    struct pointer* (*free)(u64 address);
    struct pointer* (*read)(u64 address);
    u64 (*write)(struct pointer* value);
#ifdef VM_DEBUG_INFO
    void (*dump)(void);
    void (*dump_ref)(void);
#endif
};

struct vm_type {
    void (*free)(u64 args);
};

#endif /* _VIRTUAL_H_ */
