#include "data.h"

/* queue/list: vtable definition */
struct list_vtable {
    /* initialize context */
    void (*init)(struct list** const current);
    /* push item on current context (stack) */
    void (*push)(struct list** const current, void* item);
    /* pop item on current context (stack) */
    void* (*pop)(struct list** const current);
    /* peek item on current context (stack) */
    void* (*peek)(struct list** const current);
    /* destroy context */
    void (*destroy)(struct list** const current);
};

/* queue/list: vtable */
const struct list_vtable list_vt;
