#ifndef _PLAYGROUND_LIST_H_
#define _PLAYGROUND_LIST_H_

#include "std/common.h"

struct list_data {
    void** ptr;
    void** base;
    void** max;
    u64 size;
};

struct list {
    struct list_data* (*alloc)(u64 size);
    void (*free)(struct list_data* pointer);
    void* (*push)(struct list_data* pointer, void* data);
    void* (*pop)(struct list_data* pointer);
    void* (*peek)(struct list_data* pointer);
};

#endif // _PLAYGROUND_LIST_H_
