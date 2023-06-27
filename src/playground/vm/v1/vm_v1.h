#ifndef _VIRTUAL_H_
#define _VIRTUAL_H_

#include "std/common.h"

struct vm_data;

struct vm {
    void (*init)(struct vm_data** current, u64 size);
    void (*destroy)(struct vm_data** current);
    struct pointer* (*free)(struct vm_data** current, u64 address);
    struct pointer* (*read)(struct vm_data** current, u64 address);
    u64 (*write)(struct vm_data** current, struct pointer* value);
#ifdef USE_MEMORY_DEBUG_INFO
    void (*dump)(struct vm_data** current);
    void (*dump_ref)(struct vm_data** current);
#endif
};

/* type definition */
typedef void (*function)(u64 args);

#endif /* _VIRTUAL_H_ */
