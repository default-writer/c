#include <stdlib.h>
#include "list.h"

// queue/list: vtable definition0
struct memory_manager_vtable {
    // returns next memory pointer
    struct list* (*alloc)(struct list_context* const ctx, size_t nmemb, size_t size);
    // releases memory pointer
    void* (*free)(struct list_context* const ctx, struct list* const pointer);
};

// queue/list: vtable
const struct memory_manager_vtable list_light_vt;
