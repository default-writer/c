#include <stdlib.h>
#include "list.h"

// queue/list context
struct memory_manager_context { 
    // alloc buffer list
    struct list* alloc;
    // free buffer list
    struct list* free;
    // available
    size_t available;
    // free
    size_t used;
};

// queue/list: vtable definition0
struct memory_manager_vtable {
    // initialize context
    void (*init)(struct memory_manager_context* const ctx);
    // returns next memory pointer
    struct list* (*alloc)(struct memory_manager_context* const ctx, size_t nmemb, size_t size);
    // releases memory pointer
    void (*free)(struct memory_manager_context* const ctx, struct list** const pointer);
    // destroy context
    void (*destroy)(struct memory_manager_context* const ctx);
};

// queue/list: vtable
const struct memory_manager_vtable list_light_vt;
