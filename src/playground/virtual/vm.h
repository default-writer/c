#ifndef _PLAYGROUND_VIRTUAL_H_
#define _PLAYGROUND_VIRTUAL_H_

#include "std/common.h"

struct vm_data;

struct vm {
    void (*init)(struct vm_data** current, u64 size);
    void (*destroy)(struct vm_data** current);
    struct pointer* (*free)(struct vm_data** current, u64 address);
    struct pointer* (*read)(struct vm_data** current, u64 address);
    u64 (*write)(struct vm_data** current, struct pointer* value);
    void (*memory_dump)(struct vm_data* vm_ptr);
    void (*memory_dump_ref)(struct vm_data* vm_ptr);
};

#endif // _PLAYGROUND_VIRTUAL_H_
