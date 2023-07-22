#ifndef _VIRTUAL_H_
#define _VIRTUAL_H_

#include "std/common.h"

struct vm;
struct vm_data;
struct pointer;

struct vm_methods {
    struct vm* (*init)(u64 size);
    void (*destroy)(struct vm**);
    u64 (*alloc)(struct pointer* value);
    struct pointer* (*free)(u64 address);
    struct pointer* (*read)(u64 address, u64 typeid);
#ifdef VM_DEBUG_INFO
    void (*dump)(void);
    void (*dump_ref)(void);
#endif
};

struct vm_type {
    u64 (*free)(u64 args);
    const u64 typeid;
};

#endif /* _VIRTUAL_H_ */
