#ifndef _PLAYGROUND_VIRTUAL_H_
#define _PLAYGROUND_VIRTUAL_H_

#include "std/common.h"

#include "enumerator/enumerator.h"

struct vm_data {
    void** sp; // stack pointer
    void** bp; // base pointer
    struct vm_data* next;
    struct vm_data* prev;
    u64 address_space;
    u64 size;
};

struct vm_data_enumerator {
    void* (*next)(struct vm_data**, struct enumerator_data* ptr);
};

struct vm {
    void (*init)(struct vm_data** current, u64 size);
    void (*destroy)(struct vm_data** current);
    void* (*free)(struct vm_data** current, u64 address);
    void* (*read)(struct vm_data** current, u64 address);
    u64 (*write)(struct vm_data** current, void* value);
};

struct enumerator_data* vm_enumerator_init(struct vm_data** current);
void vm_enumerator_destroy(struct enumerator_data* data);

#endif // _PLAYGROUND_VIRTUAL_H_
