#ifndef _DATA_H_
#define _DATA_H_

/* queue/list: prev, next, payload */
struct list_data {
    /* points to previous node */
    struct list_data* prev;
    /* payload */
    void*  payload;
};

/* queue/list: vtable definition */
struct list_methods_tiny {
    /* initialize context */
    void (*init)(struct list_data** const current);
    /* destroy context */
    void (*destroy)(struct list_data** const current);
    /* push item on current context (stack) */
    void (*push)(struct list_data** const current, void* item);
    /* pop item on current context (stack) */
    void* (*pop)(struct list_data** const current);
    /* peek item on current context (stack) */
    void* (*peek)(struct list_data** const current);
};

#endif // _DATA_H_