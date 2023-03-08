#include "playground/memory/v4/memory_v4.h"

#define MAX_MEMORY 0xffff // 64K bytes

#include "common/alloc.h"

#include "memory/ref.h"

#include "playground/memory/list/v1/memory_list_v1.h"

// offset for memory_ref structure
extern struct memory_ref_methods memory_ref_definition;

static struct memory_ref_methods* ref = &memory_ref_definition;

// global allocated memory
static struct memory_ref* memory;
static void** current = 0;

/* api */

static void memory_init(void);
static void memory_destroy(void);
static void* memory_alloc(u64 size);
static void memory_free(void* data);

/* declaration */

static void* memory_alloc_internal(void* data, u64 size);
static void memory_free_internal(void* data);

/* implementation */

static void* memory_alloc_internal(void* data, u64 size) {
    struct memory_ref* ptr = ref->ref(data);
    if (ptr != 0) {
        void** tmp = ref->alloc(size);
        ptr->next = ref->ptr(tmp);
#ifdef USE_MEMORY_DEBUG_INFO
        printf("  p.: 0x%016llx .0x%016llx .0x%016llx\n", (u64)data, (u64)ptr->prev, (u64)ptr->next);
#endif
        struct memory_ref* _current = ref->ref(ptr->next);
        _current->size = size;
        _current->prev = ref->ptr(ptr);
        ptr = ptr->next;
    }
    return ptr;
}

// releases global memory
static void memory_free_internal(void* data) {
    void** head = data;
    u64 size = ref->size(data);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("  0-: 0x%016llx !  %16lld\n", (u64)data, size);
#endif
    ref->free(head);
}

static void memory_init(void) {
    memory = _list_alloc(sizeof(struct memory_ref));
    ++memory;
    current = memory_alloc_internal(memory, 0);
    memory_list_init();
}

static void memory_destroy(void) {
    void* data = 0;
    while ((data = memory_list_pop()) != 0) {
        memory_free_internal(data);
    }
    memory_list_destroy();
    --memory;
    _list_free(ref->ref(memory->next), sizeof(struct memory_ref));
    _list_free(memory, sizeof(struct memory_ref));
#ifdef USE_MEMORY_CLEANUP
    memory = 0;
    current = 0;
#endif
}

static void* memory_alloc(u64 size) {
    void* tmp = memory_list_peek();
    void** data = 0;
    u64 cached_size = ref->size(tmp);
    if (tmp != 0 && cached_size >= size) {
        data = memory_list_pop();
        struct memory_ref* ptr = ref->ref(data);
        if (ptr != 0) {
#ifdef USE_MEMORY_DEBUG_INFO
            printf("  0*: 0x%016llx >  %16lld\n", (u64)data, cached_size);
#endif
        }
    } else {
        current = memory_alloc_internal(current, size);
        data = current;
        struct memory_ref* ptr = ref->ref(data);
        if (ptr != 0) {
#ifdef USE_MEMORY_DEBUG_INFO
            printf("  0+: 0x%016llx >  %16lld\n", (u64)data, size);
#endif
        }
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
