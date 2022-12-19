#ifndef _EXPERIMENTAL_DATA_H_
#define _EXPERIMENTAL_DATA_H_

#include "std/types.h"

struct list_data {
    /* payload */
    void** data;
};

struct list_methods {
    /* push item on current context (stack) */
    void (*push)(struct list_data** const current, void* item);
    /* pop item on current context (stack) */
    void* (*pop)(struct list_data** const current);
    /* peek item on current context (stack) */
    void* (*peek)(struct list_data** const current);
};

#endif // _EXPERIMENTAL_DATA_H_