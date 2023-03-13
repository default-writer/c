#include "playground/memory/api/v2/memory.h"

#include "memory/api/v1/ref.h"

#include "playground/memory/list/v1/memory_list_v1.h"

#define MAX_MEMORY 0xffff // 64K bytes

// offset for memory_ref structure
extern struct memory_ref_methods memory_ref_definition;

static struct memory_ref_methods* ref = &memory_ref_definition;

/* api */

static void memory_init(void);
static void memory_destroy(void);
static void* memory_alloc(u64 size);
static void memory_free(void* data);

/* declaration */

/* implementation */

static void memory_init(void) {
    ref->init();
    memory_list_init();
}

static void memory_destroy(void) {
    void* data = 0;
    while ((data = memory_list_pop()) != 0) {
        ref->free(data);
    }
    memory_list_destroy();
    ref->destroy();
}

static void* memory_alloc(u64 size) {
    void* tmp = memory_list_peek();
    void* data = 0;
    u64 ptr_size = ref->size(tmp);
    if (ptr_size != 0 && ptr_size >= size) {
        data = memory_list_pop();
    } else {
        data = ref->alloc(size);
    }
    return data;
}

// releases global memory
static void memory_free(void* data) {
    memory_list_push(data);
}

/* public */

const struct memory_allocator memory_allocator_v4 = {
    .init = memory_init,
    .destroy = memory_destroy,
    .alloc = memory_alloc,
    .free = memory_free
};
