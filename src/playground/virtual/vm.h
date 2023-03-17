#ifndef _PLAYGROUND_VIRTUAL_H_
#define _PLAYGROUND_VIRTUAL_H_

#include "std/common.h"

#include "enumerator/enumerator.h"

struct vm_data {
    struct pointer** sp; // stack pointer
    struct pointer** bp; // base pointer
    struct vm_data* next;
    struct vm_data* prev;
    u64 address_space;
    u64 size;
};

struct vm_data_enumerator {
    struct pointer* (*next)(void);
};

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
