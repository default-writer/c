#include "common/alloc.h"

#include "memory/ref.h"

#include "common/alloc.h"

/* declaration */

static const u64 memory_offset;

static struct memory_ref* memory_ref_ref(void* data);
static void* memory_ref_ptr(struct memory_ref* data);
static u64 memory_ref_size(void* data);
static struct memory_ref* memory_ref_alloc(u64 size);

/* implementation */

static struct memory_ref* memory_ref_ref(void* data) {
    struct memory_ref* ptr = 0;
    if (data != 0) {
        ptr = (struct memory_ref*)data;
        --ptr;
    }
    return ptr;
}

static void* memory_ref_ptr(struct memory_ref* data) {
    struct memory_ref* ptr = 0;
    if (data != 0) {
        ptr = data;
        ++ptr;
    }
    return (void*)ptr;
}

static u64 memory_ref_size(void* data) {
    u64 size = 0;
    if (data != 0) {
        struct memory_ref* ptr = memory_ref_ref(data);
        size = ptr->size;
    }
    return size;
}

static struct memory_ref* memory_ref_alloc(u64 size) {
    struct memory_ref* data = _list_alloc((size + memory_offset) * sizeof(void*));
#ifdef USE_MEMORY_DEBUG_INFO
    printf("  0+: 0x%016llx !  %16lld\n", (u64)data, size);
#endif
    return data;
}

static void memory_ref_free(void* data) {
    void** ptr = data;
    u64 size = memory_ref_size(data);
    _list_free(ptr - memory_offset, (size + memory_offset) * sizeof(void*));
#ifdef USE_MEMORY_DEBUG_INFO
    printf("  0-: 0x%016llx !  %16lld\n", (u64)data, size);
#endif
}

/* public */

static const u64 memory_offset = sizeof(struct memory_ref) / sizeof(void*);

const struct memory_ref_methods memory_ref_definition = {
    .ref = memory_ref_ref,
    .ptr = memory_ref_ptr,
    .alloc = memory_ref_alloc,
    .free = memory_ref_free
};
