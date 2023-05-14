#ifndef _LIST_V2_H_
#define _LIST_V2_H_

#include "std/common.h"

struct list_data;

struct list {
    struct list_data* (*alloc)(u64 size);
    void (*free)(struct list_data* pointer);
    void* (*push)(struct list_data* pointer, void* data);
    void* (*pop)(struct list_data* pointer);
    void* (*peek)(struct list_data* pointer);
};

#endif /* _LIST_V2_H_ */
