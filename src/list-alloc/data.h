#ifndef _LIST_ALLOC_DATA_H_
#define _LIST_ALLOC_DATA_H_

#include "std/common.h"

struct list_data {
    /* payload size */
    u64 size;
    /* data */
    void** data;
};

struct list {
    /* push item on current context (stack) */
    void (*push)(struct list_data** current, void* item);
    /* pop item on current context (stack) */
    void* (*pop)(struct list_data** current);
    /* peek item on current context (stack) */
    void* (*peek)(struct list_data** current);
#ifdef USE_MEMORY_DEBUG_INFO
    /* print head */
    void (*print_head)(struct list_data** current);
    /* print */
    void (*print)(struct list_data** current);
#endif
};

#endif /* _LIST_ALLOC_DATA_H_ */
