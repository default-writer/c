#include "playground/memory/v4/memory_v4.h"

#define MAX_MEMORY 0xffff // 64K bytes

#include "common/alloc.h"

#include "memory/ref.h"

#include "playground/memory/list/memory_list.h"

// global allocated memory
static struct memory_ref* memory;
static void** current = 0;

/* api */

static void memory_init(void);
static void memory_destroy(void);
static void* memory_alloc(u64 size);
static void memory_free(void* data);

/* declaration */

static struct memory_ref* _ref(void* data);
static void* _ptr(void* data);
static u64 _sizeof(void* data);
static void* memory_alloc_internal(void* data, u64 size);
static void memory_free_internal(void* data);

/* implementation */

const u64 memory_offset = sizeof(struct memory_ref) / sizeof(void*);

static struct memory_ref* _ref(void* data) {
    struct memory_ref* ptr = 0;
    if (data != 0) {
        ptr = (struct memory_ref*)data;
        --ptr;
    }
    return ptr;
}

static void* _ptr(void* data) {
    struct memory_ref* ptr = 0;
    if (data != 0) {
        ptr = (struct memory_ref*)data;
        ++ptr;
    }
    return (void*)ptr;
}

static u64 _sizeof(void* data) {
    u64 size = 0;
    if (data != 0) {
        struct memory_ref* ptr = _ref(data);
        size = ptr->size;
    }
    return size;
}

static void* memory_alloc_internal(void* data, u64 size) {
    struct memory_ref* ptr = _ref(data);
    void** tmp = _list_alloc((size + memory_offset) * sizeof(void*));
    if (ptr != 0) {
        ptr->next = _ptr(tmp);
#ifdef USE_MEMORY_DEBUG_INFO
        printf("  p.: 0x%016llx .0x%016llx .0x%016llx\n", (u64)data, (u64)ptr->prev, (u64)ptr->next);
#endif
    }
    struct memory_ref* _current = _ref(ptr->next);
    _current->size = size;
    _current->prev = _ptr(ptr);
    return tmp + memory_offset;
}

// releases global memory
static void memory_free_internal(void* data) {
    void** head = data;
    u64 size = _sizeof(data);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("  0-: 0x%016llx !  %16lld\n", (u64)data, size);
#endif
    _list_free(head - memory_offset, (size + memory_offset) * sizeof(void*));
}

static void memory_init(void) {
    memory = _list_alloc(sizeof(struct memory_ref));
    ++memory;
    current = memory_alloc_internal(memory, 0);
    list_init();
}

static void memory_destroy(void) {
    void* data = 0;
    while ((data = list_pop()) != 0) {
        memory_free_internal(data);
    }
    list_destroy();
    --memory;
    _list_free(_ref(memory->next), sizeof(struct memory_ref));
    _list_free(memory, sizeof(struct memory_ref));
#ifdef USE_MEMORY_CLEANUP
    memory = 0;
    current = 0;
#endif
}

static void* memory_alloc(u64 size) {
    void* tmp = list_peek();
    void** data = 0;
    u64 cached_size = _sizeof(tmp);
    if (tmp != 0 && cached_size >= size) {
        data = list_pop();
        struct memory_ref* ptr = _ref(data);
        if (ptr != 0) {
#ifdef USE_MEMORY_DEBUG_INFO
            printf("  0*: 0x%016llx >  %16lld\n", (u64)data, cached_size);
#endif
        }
    } else {
        current = memory_alloc_internal(current, size);
        data = current;
        struct memory_ref* ptr = _ref(data);
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
    list_push(data);
}

/* public */

const struct memory_allocator memory_allocator_v4 = {
    .init = memory_init,
    .destroy = memory_destroy,
    .alloc = memory_alloc,
    .free = memory_free
};
