#include "playground/memory/api/v2/memory.h"

#include "memory/api/v2/ref.h"

#define MAX_MEMORY 0xfffffc // 16777215 (0xffffff) octa-bytes or 134217720 bytes (128MB)

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
    ref->init(0);
}

static void memory_destroy(void) {
    ref->destroy();
}

static void* memory_alloc(u64 size) {
    return ref->alloc(size);
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
