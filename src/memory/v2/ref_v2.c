#include "memory/api/v2/ref_v2.h"
#include "common/alloc.h"
#include "playground/memory/list/v2/memory_list_v2.h"

/* declaration */

/* size of a memory block to allocate */
static const size_t _size = sizeof(struct memory_ref);

static struct memory_ref* memory;

/* global allocated memory */
static void** current = 0;

static struct memory_ref* memory_ref_ref(void* data);
static void* memory_ref_ptr(struct memory_ref* data);
static u64 memory_ref_size(void* data);
static void* memory_ref_alloc(u64 size);
static void memory_ref_init(u64 size);
static void memory_ref_destroy(void);
static void memory_ref_push(void* data);
static void* memory_ref_pop(void);
static void* memory_ref_peek(void);

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
        const struct memory_ref* ptr = memory_ref_ref(data);
        size = ptr->size;
    }
    return size;
}

static void* memory_ref_alloc(u64 size) {
    void* data = current;
    void* ptr = 0;
    if (data != 0) {
        struct memory_ref* ref_ptr = memory_ref_ref(data);
        struct memory_ref* tmp = global_alloc(_size + size * sizeof(void*));
        ref_ptr->next = memory_ref_ptr(tmp);
#ifdef USE_MEMORY_DEBUG_INFO
        printf("  p.: %016llx . %016llx . %016llx\n", (u64)data, (u64)ref_ptr->prev, (u64)ref_ptr->next);
#endif
        struct memory_ref* _current = memory_ref_ref(ref_ptr->next);
        _current->size = size;
        _current->prev = memory_ref_ptr(ref_ptr);
        ptr = ref_ptr->next;
        current = ptr; /* advance current ptr to the new dat */
    }
    return ptr;
}

static void memory_ref_free(void* data) {
    if (data != 0) {
        u8* ptr = (u8*)data - _size;
        u64 size = memory_ref_size(data) * sizeof(void*);
        global_free(ptr, size + _size);
#ifdef USE_MEMORY_DEBUG_INFO
        printf("  0-: %016llx ! %16lld\n", (u64)data, size);
#endif
    }
}

static void memory_ref_init(u64 size) {
    memory_list_init();
    memory = global_alloc(_size);
    ++memory;
    current = (void*)memory;
    current = memory_ref_alloc(size);
}

static void memory_ref_destroy(void) {
    void* data = 0;
    while ((data = memory_ref_pop()) != 0) {
        memory_ref_free(data);
    }
    --memory;
    global_free(memory_ref_ref(memory->next), _size);
    global_free(memory, _size);
#ifdef USE_MEMORY_CLEANUP
    memory = 0;
    current = 0;
#endif
    memory_list_destroy();
}

static void memory_ref_push(void* data) {
    if (data != 0) {
        memory_list_push(memory_ref_ref(data));
    }
}

static void* memory_ref_pop(void) {
    void* data = memory_list_pop();
    void* ptr = 0;
    if (data != 0) {
        ptr = memory_ref_ptr(data);
    }
    return ptr;
}

static void* memory_ref_peek(void) {
    void* data = memory_list_peek();
    void* ptr = 0;
    if (data != 0) {
        ptr = memory_ref_ptr(data);
    }
    return ptr;
}

/* public */

const struct memory_ref_methods memory_ref_definition_v2 = {
    .init = memory_ref_init,
    .destroy = memory_ref_destroy,
    /* .ref = memory_ref_ref, */
    /* .ptr = memory_ref_ptr, */
    .size = memory_ref_size,
    .alloc = memory_ref_alloc,
    .free = memory_ref_free,
    .push = memory_ref_push,
    .pop = memory_ref_pop,
    .peek = memory_ref_peek
};
