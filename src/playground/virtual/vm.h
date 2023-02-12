#ifndef _PLAYGROUND_VIRTUAL_H_
#define _PLAYGROUND_VIRTUAL_H_

#include "std/common.h"

struct vm_data {
    void** ptr;
    void** base;
    void** max;
    u64 size;
};

struct vm {
    struct vm_data* (*init)(u64 size);
    void (*destroy)(struct vm_data* pointer);
    u64 (*alloc)(struct vm_data* pointer);
    void (*free)(struct vm_data* pointer, u64 address);
    void* (*read)(struct vm_data* pointer, u64 address);
    void (*write)(struct vm_data* pointer, u64 address, void* value);
};

#endif // _PLAYGROUND_VIRTUAL_H_