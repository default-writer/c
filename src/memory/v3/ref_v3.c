#include "common/alloc.h"
#include "memory/api/v3/ref.h"
#include "playground/memory/list/v3/memory_list_v3.h"
#include "std/macros.h"

/* declaration */

static const u64 memory_offset;

static struct memory_ref* memory_ptr;

struct memory_ref_ptr {
    struct memory_ref* ptr;
    u64 size;
};

static void* memory_ref_alloc(u64 size);
static void memory_ref_init(u64 size);
static void memory_ref_destroy(void);
static void memory_ref_free(void* data);

static struct memory_ref* memory_alloc_internal(u64 size);
static void memory_ref_free_internal(void* data);
static void* memory_ref_ptr_internal(struct memory_ref* ptr);
static void* memory_ref_peek_internal(void);
static void* memory_ref_pop_internal(void);

/* implementation */

static void* memory_ref_ptr_internal(struct memory_ref* ptr) {
    void* data = 0;
    if (ptr != 0) {
        struct memory_ref* tmp = ptr;
        data = (void*)++tmp;
    }
    return data;
}

static void* memory_ref_alloc(u64 size) {
    struct memory_ref* ptr = memory_ref_peek_internal();
    void* data = 0;
    if (ptr != 0 && size > 0 && ptr->size < size) {
        ptr = memory_alloc_internal(size);
    }
    ptr->size -= size;
#ifdef USE_MEMORY_DEBUG_INFO
    printf("  c.: 0x%016llx .0x%016llx >  %16lld !  %16lld\n", (u64)ptr, (u64)ptr->cache, ptr->address_space, ptr->size); // NOLINT
#endif
    data = memory_ref_ptr_internal(ptr);
    data = (u64*)data + ptr->size;
    return data;
}

static void memory_ref_free_internal(void* data) {
    if (data != 0) {
        struct memory_ref* ptr = data;
        _list_free(ptr, (ptr->size + memory_offset) * sizeof(void*));
    }
}

static void memory_ref_init(u64 size) {
    memory_list_init();
    memory_ptr = _list_alloc(sizeof(struct memory_ref));
    if (size > 0) {
        memory_ptr->next = _list_alloc((size + memory_offset) * sizeof(void*));
        memory_ptr->next->size = size;
        memory_ptr->next->address_space = size;
        memory_list_push(memory_ptr->next);
        // memory_alloc_internal(size);
    }
}

static void memory_ref_destroy(void) {
    struct memory_ref* ptr = 0;
    while ((ptr = memory_ref_pop_internal()) != 0) {
        memory_ref_free_internal(ptr);
    }
    _list_free(memory_ptr, sizeof(struct memory_ref));
#ifdef USE_MEMORY_CLEANUP
    memory_ptr = 0;
#endif
    memory_list_destroy();
}

static struct memory_ref* memory_alloc_internal(u64 size) {
    struct memory_ref* ptr = memory_list_peek();
    void* data = ptr;
    struct memory_ref* next_ptr = _list_alloc((size + memory_offset) * sizeof(void*));
    next_ptr->size = size;
    next_ptr->address_space = size;
    next_ptr->prev = memory_ref_ptr_internal(ptr);
    if (ptr != 0) {
        ptr->next = memory_ref_ptr_internal(next_ptr);
#ifdef USE_MEMORY_DEBUG_INFO
        printf("  p.: 0x%016llx .0x%016llx .0x%016llx\n", (u64)data, (u64)ptr->prev, (u64)ptr->next);
#endif
    }
    ptr = next_ptr;
    memory_list_push(ptr);
    return ptr;
}

static void memory_ref_free(void* data) {
    if (data != 0) {
        struct memory_ref* ptr = memory_list_peek();
        void* current = memory_ref_ptr_internal(ptr);
        if (data == current) {
            ptr->size = ptr->address_space;
        }
    }
}

static void* memory_ref_pop_internal(void) {
    struct memory_ref* ptr = 0;
    void* data = memory_list_pop();
    if (data != 0) {
        ptr = data;
    }
    return ptr;
}

static void* memory_ref_peek_internal(void) {
    void* data = memory_list_peek();
    void* ptr = 0;
    if (data != 0) {
        ptr = data;
    }
    return ptr;
}

/* public */

static const u64 memory_offset = sizeof(struct memory_ref) / sizeof(void*);

const struct memory_ref_methods memory_ref_definition = {
    .init = memory_ref_init,
    .destroy = memory_ref_destroy,
    .alloc = memory_ref_alloc,
    .free = memory_ref_free
};
