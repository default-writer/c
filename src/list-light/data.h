#ifndef _LIST_LIGHT_H_
#define _LIST_LIGHT_H_

#include "std/common.h"

struct list_data {
    /* points to previous node */
    struct list_data* prev;
    /* points to next node */
    struct list_data* next;
    /* data */
    const void* data;
};

struct list {
    /* initialize context */
    void (*init)(struct list_data** const current, struct list_data* (*new)());
    /* destroy context */
    void (*destroy)(struct list_data** const current, void (*delete)(struct list_data*), struct list_data* (*next)(struct list_data*));
    /* push item on current context (stack) */
    void (*push)(struct list_data** const current, const void* item);
    /* pop item on current context (stack) */
    const void* (*pop)(struct list_data** const current);
    /* peek item on current context (stack) */
    const void* (*peek)(struct list_data** const current);
};

#endif // _LIST_LIGHT_H_