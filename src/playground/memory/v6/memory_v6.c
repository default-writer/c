#include "playground/memory/api/v2/memory.h"

#define MAX_MEMORY 0xfffffc // 16777215 (0xffffff) octa-bytes or 134217720 bytes (128MB)

#include "common/alloc.h"

#include "memory/ref.h"

// offset for memory_ref structure
extern struct memory_ref_methods memory_ref_definition;

static struct memory_ref_methods* ref = &memory_ref_definition;

/* api */

static void memory_init(void);
static void memory_destroy(void);
static void* memory_alloc(u64 size);
static void memory_free(void* data);

/* declaration */
static void* memory;

/* implementation */

static void memory_init(void) {
    ref->init();
    memory = ref->alloc(MAX_MEMORY);
}

static void memory_destroy(void) {
    ref->free(memory);
    ref->destroy();
}

static void* memory_alloc(u64 size) {
    void* tmp = ref->peek();
    void* data = 0;
    u64 ptr_size = ref->size(tmp);
    if (ptr_size != 0 && ptr_size >= size) {
        data = ref->pop();
    } else {
        data = ref->alloc(size);
    }
    return data;
}

// releases global memory
static void memory_free(void* data) {
    ref->push(data);
}

/* public */

const struct memory_allocator memory_allocator_v5 = {
    .init = memory_init,
    .destroy = memory_destroy,
    .alloc = memory_alloc,
    .free = memory_free
};
