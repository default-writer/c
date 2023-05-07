#ifndef _PLAYGROUND_VM_V2_H_
#define _PLAYGROUND_VM_V2_H_

#include "std/common.h"

struct vm_data;

struct vm {
    void (*init)(struct vm_data** current, u64 size);
    void (*destroy)(struct vm_data** current);
    struct pointer* (*free)(struct vm_data** current, u64 address);
    struct pointer* (*read)(struct vm_data** current, u64 address);
    u64 (*write)(struct vm_data** current, struct pointer* value);
#ifdef USE_MEMORY_DEBUG_INFO
    void (*memory_dump)(struct vm_data* vm_ptr);
#endif
};

#endif /* _PLAYGROUND_VM_V2_H_ */
