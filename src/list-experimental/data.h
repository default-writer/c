#ifndef _LIST_EXPERIMENTAL_DATA_H_
#define _LIST_EXPERIMENTAL_DATA_H_

#include "std/common.h"

/* defines N-element collection*/
#define N 8

struct list_data {
    /* data */
    void** data;
    /* next element */
    struct list_data* next;
};

struct list {
    /* push item on current context (stack) */
    void (*push)(struct list_data** current, void* item);
    /* pop item on current context (stack) */
    void* (*pop)(struct list_data** current);
    /* peek item on current context (stack) */
    void* (*peek)(struct list_data** current);
};

#endif // _LIST_EXPERIMENTAL_DATA_H_