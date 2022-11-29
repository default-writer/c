#ifndef _DATA_H_
#define _DATA_H_

struct list_data {
    /* points to previous node */
    struct list_data* prev;
    /* payload */
    void* payload;
};

/* queue/list: vtable definition */
struct list_methods_micro {
    /* push item on current context (stack) */
    void (*push)(struct list_data** const current, void* item);
    /* pop item on current context (stack) */
    void* (*pop)(struct list_data** const current);
    /* initialize context */
    void (*init)(struct list_data** const current);
    /* destroy context */
    void (*destroy)(struct list_data** const current);
};

struct list_class_micro {
    /* link to self-contained methods structure */
    const struct list_methods_micro* methods;
};

#endif // _DATA_H_