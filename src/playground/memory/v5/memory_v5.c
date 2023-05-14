#include "memory/api/v2/ref_v2.h"
#include "playground/memory/api/memory.h"

#define MAX_MEMORY 0xffff /* 64K bytes */

/* offset for memory_ref structure */
extern struct memory_ref_methods memory_ref_definition_v2;

static struct memory_ref_methods* ref = &memory_ref_definition_v2;

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

/* releases global memory */
static void memory_free(void* data) {
    ref->push(data);
}

/* public */

const union memory_allocator_api memory_allocator_v5 = {
    .v2.init = memory_init,
    .v2.destroy = memory_destroy,
    .v2.alloc = memory_alloc,
    .v2.free = memory_free
};
