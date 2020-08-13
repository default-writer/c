#include "list.h"

// queue/list: vtable definition
struct list_light_vtable {
    // push item on current context (stack)
    void (*push)(struct list_context* const ctx, void* item);
    // pop item on current context (stack)
    void* (*pop)(struct list_context* const ctx);
    // peek item on current context (stack)
    void* (*peek)(struct list_context* const ctx);
    // initialize context
    void (*init)(struct list_context* const ctx);
    // destroy context
    void (*destroy)(struct list_context* const ctx);
};

// queue/list: vtable
const struct list_light_vtable list_light_vt;
