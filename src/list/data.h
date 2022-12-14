#ifndef _LIST_DATA_H_
#define _LIST_DATA_H_

#include <stdlib.h>

struct list_data {
    /* points to previous node */
    struct list_data* prev;
    /* points to next node */
    struct list_data* next;
    /* payload */
    void* payload;
};

struct list_methods {
    /* allocate list item */
    struct list_data* (*alloc)(void* payload);
    /* free item */
    void (*free)(struct list_data** const item);
    /* initialize context */
    void (*init)(struct list_data** const current, struct list_data* (*new)());
    /* destroy context */
    void (*destroy)(struct list_data** const current, void (*delete)(struct list_data*), struct list_data* (*next)(struct list_data*));
    /* push item on current context (stack) */
    struct list_data* (*push)(struct list_data** const current, struct list_data* const item);
    /* pop item on current context (stack) */
    struct list_data* (*pop)(struct list_data** const current);
    /* peek item on current context (stack) */
    struct list_data* (*peek)(struct list_data** const current);
};

#endif // _LIST_DATA_H_