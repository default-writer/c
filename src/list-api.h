#include "list.h"

// queue/list: vtable definition
struct list_vtable {
    // push item on current context (stack)
    struct list* (*push)(struct list_context* const ctx, struct list** const item);
    // pop item on current context (stack)
    struct list* (*pop)(struct list_context* const ctx);
    // peek item on current context (stack)
    struct list* (*peek)(struct list_context* const ctx);
    // root item on current context (stack)
    struct list* (*root)(struct list_context* const ctx);
    // free item on current context (stack)
    void (*free)(struct list_context* const ctx, struct list** const item);
    // alloc item on current context (stack)
    void (*alloc)(struct list_context* const ctx, void* payload);
    // initialize context
    void (*init)(struct list_context* const ctx);
    // destroy context
    void (*destroy)(struct list_context* const ctx);
};

// queue/list: vtable
const struct list_vtable list_vt;
