#ifndef _LIST_EXPERIMENTAL_DATA_H_
#define _LIST_EXPERIMENTAL_DATA_H_

#include "std/types.h"

struct list_data {
    /* payload */
    void** data;
    SIZE size;
};

struct list_methods {
    /* push item on current context (stack) */
    void (*push)(struct list_data** const current, const void* item);
    /* pop item on current context (stack) */
    const void* (*pop)(struct list_data** const current);
    /* peek item on current context (stack) */
    const void* (*peek)(struct list_data** const current);
};

#endif // _LIST_EXPERIMENTAL_DATA_H_