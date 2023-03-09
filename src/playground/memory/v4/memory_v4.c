#include "playground/memory/v4/memory_v4.h"

#define MAX_MEMORY 0xffff // 64K bytes

#include "common/alloc.h"

#include "memory/ref.h"

#include "playground/memory/list/v1/memory_list_v1.h"

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
    struct memory_ref* ptr = ref->ref(tmp);
    if (ptr != 0 && ptr->size >= size) {
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
