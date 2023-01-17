#ifndef _LIST_MICRO_DATA_H_
#define _LIST_MICRO_DATA_H_

#include "std/common.h"

struct list_data {
    /* points to previous node */
    struct list_data* next;
    /* data */
    void* data;
};

struct list {
    /* initialize context */
    void (*init)(struct list_data** current);
    /* destroy context */
    void (*destroy)(struct list_data** current);
    /* push item on current context (stack) */
    void (*push)(struct list_data** current, void* item);
    /* pop item on current context (stack) */
    void* (*pop)(struct list_data** current);
    /* peek item on current context (stack) */
    void* (*peek)(struct list_data** current);
    /* print head */
    void (*print_head)(struct list_data**  current);
    /* print */
    void (*print)(struct list_data**  current);
};

#endif // _LIST_MICRO_DATA_H_