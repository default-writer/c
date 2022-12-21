#ifndef _LIST_MICRO_DATA_H_
#define _LIST_MICRO_DATA_H_

#include "std/types.h"

struct list_data {
    /* points to previous node */
    struct list_data* next;
    /* payload */
    const void* payload;
};

struct list_methods {
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

struct list_data* _new();
void _delete(struct list_data* ptr);

#endif // _LIST_MICRO_DATA_H_