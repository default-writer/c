#ifndef _LIST_ALLOC_DATA_H_
#define _LIST_ALLOC_DATA_H_

#include "std/common.h"

struct list_data {
    /* payload size */
    SIZE size;
    /* data */
    void** data;
    /* next element */
    struct list_data* next;
};

struct list {
    /* push item on current context (stack) */
    void (*push)(struct list_data** const current, const void* item);
    /* pop item on current context (stack) */
    const void* (*pop)(struct list_data** const current);
    /* peek item on current context (stack) */
    const void* (*peek)(struct list_data** const current);
};

#endif // _LIST_ALLOC_DATA_H_