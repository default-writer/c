#ifndef _DATA_H_
#define _DATA_H_

/* queue/list: prev, next, payload */
struct list_data {
    /* points to previous node */
    struct list_data* prev;
    /* points to next node */
    struct list_data* next;
    /* payload */
    void*  payload;
};

/* queue/list: vtable definition */
struct list_methods {
    /* initialize context */
    void (*init)(struct list_data** const current);
    /* destroy context */
    void (*destroy)(struct list_data** const current);
    /* alloc item on current context (stack) */
    void (*alloc)(struct list_data** const current, void* payload);
    /* push item on current context (stack) */
    struct list_data* (*push)(struct list_data** const current, struct list_data** const item);
    /* pop item on current context (stack) */
    struct list_data* (*pop)(struct list_data** const current);
    /* peek item on current context (stack) */
    struct list_data* (*peek)(struct list_data** const current);
    /* free item on current context (stack) */
    void (*free)(struct list_data** const current, struct list_data** const item);
};

#endif // _DATA_H_