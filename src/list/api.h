#include "data.h"

/* queue/list: vtable definition */
struct list_vtable {
    /* initialize context */
    void (*init)(struct list** const current);
    /* alloc item on current context (stack) */
    void (*alloc)(struct list** const current, void* payload);
    /* push item on current context (stack) */
    struct list* (*push)(struct list** const current, struct list** const item);
    /* pop item on current context (stack) */
    struct list* (*pop)(struct list** const current);
    /* peek item on current context (stack) */
    struct list* (*peek)(struct list** const current);
    /* free item on current context (stack) */
    void (*free)(struct list** const current, struct list** const item);
    /* destroy context */
    void (*destroy)(struct list** const current);
};

/* queue/list: vtable */
const struct list_vtable list_vt;
