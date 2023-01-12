#ifndef _LIST_DATA_H_
#define _LIST_DATA_H_

#include "std/common.h"

struct list_data {
    /* points to previous node */
    struct list_data* prev;
    /* points to next node */
    struct list_data* next;
    /* data */
    void* data;
};

struct list {
    /* allocate list item */
    struct list_data* (*alloc)(void* payload);
    /* free item */
    void (*free)(struct list_data** item);
    /* initialize context */
    void (*init)(struct list_data** current, struct list_data* (*new)());
    /* destroy context */
    void (*destroy)(struct list_data** current, void (*delete)(struct list_data*), struct list_data* (*next)(struct list_data*));
    /* push item on current context (stack) */
    struct list_data* (*push)(struct list_data** current, struct list_data* item);
    /* pop item on current context (stack) */
    struct list_data* (*pop)(struct list_data** current);
    /* peek item on current context (stack) */
    struct list_data* (*peek)(struct list_data** current);
};

#endif // _LIST_DATA_H_