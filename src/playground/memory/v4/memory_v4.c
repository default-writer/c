#include "playground/memory/v4/memory_v4.h"

#define MAX_MEMORY 0xffff // 64K bytes

#include "common/alloc.h"

#include "list-micro/data.h"

/* list definition */
extern const struct list list_micro_definition;

/* list definition */
static const struct list* list = &list_micro_definition;
static struct list_data** cache;

// global allocated memory
static void* memory = 0;
static void** current = 0;

static void* memory_alloc_internal(u64 size);
static void memory_free_internal(void* data);

static void memory_init(void);
static void memory_destroy(void);
static void* memory_alloc(u64 size);
static void memory_free(void* data);

static void* memory_alloc_internal(u64 size) {
    void** next = current - 1;
    void* prev = current;

    void** tmp = _list_alloc((size + 3) * sizeof(void*));
    next = *next;
    *next = tmp;

    *tmp = prev;
    *(tmp + 1) = tmp + size + 2;
    *(tmp + 2) = next;

    current = tmp + 2;

    return current;
}

static u64 offset(void* data) {
    u64 size = 0;
    if (data != 0) {
        void** head = data;
        void** next = *(head - 1);
        size = (u64)(next - head);
    }
    return size;
}

static void memory_init(void) {
    current = &memory;
    *current = current;

    void** tmp = _list_alloc(3 * sizeof(void*));
    void** next = current;
    next = *next;
    *next = tmp;

    *tmp = current;
    *(tmp + 1) = tmp + 2;
    *(tmp + 2) = next;

    current = tmp + 2;

    cache = _list_alloc(sizeof(void*));
    list->init(cache);
}

static void memory_destroy(void) {
    void* data = 0;
    while ((data = list->pop(cache)) != 0) {
        memory_free_internal(data);
    }
    list->destroy(cache);
    _list_free(cache, sizeof(void*));
#ifdef USE_MEMORY_CLEANUP
    cache = 0;
#endif
    _list_free(memory, 3 * sizeof(void*));
    memory = 0;
    current = 0;
}

static void* memory_alloc(u64 size) {
    void* tmp = list->peek(cache);
    void** data = 0;
    u64 cached_size = offset(tmp);
    if (tmp != 0 && cached_size <= size) {
        data = list->pop(cache);
#ifdef USE_MEMORY_DEBUG_INFO
        printf("  0*: 0x%016llx >  %16lld\n", (u64)data, cached_size);
#endif
    } else {
        data = memory_alloc_internal(size);
#ifdef USE_MEMORY_DEBUG_INFO
        printf("  0+: 0x%016llx >  %16lld\n", (u64)data, size);
#endif
    }
    return data;
}

// releases global memory
static void memory_free(void* data) {
    void** head = data;
    void** next = *(head - 1);
    void** last = *(head - 2);
    if (*next == 0) {
        list->push(cache, data);
        void** prev = last;
        next = prev + offset(prev);
        *next = 0;
    }
}

// releases global memory
static void memory_free_internal(void* data) {
    void** head = data;
    void** last = *(head - 2);
    u64 size = offset(data);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("  0-: 0x%016llx !  %16lld\n", (u64)last, size);
#endif
#ifdef USE_MEMORY_CLEANUP
    memset(head - 2, 0, (size + 3) * sizeof(void*)); // NOLINT
#endif
    _list_free(head - 2, size);
}

const struct memory_allocator memory_allocator_v4 = {
    .init = memory_init,
    .destroy = memory_destroy,
    .alloc = memory_alloc,
    .free = memory_free
};
