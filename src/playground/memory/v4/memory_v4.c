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
static void** ptr = 0;

static void memory_init(void);
static void memory_destroy(void);
static void* memory_alloc(u64 size);
static void memory_free(void* data);

static void* alloc(void** next, void* prev, u64 size) {
    void** tmp = _list_alloc((size + 3) * sizeof(void*));
    next = *next;
    *next = tmp;

    *tmp = prev;
    *(tmp + 1) = tmp + size + 2;
    *(tmp + 2) = next;

    return tmp + 2;
}

static u64 offset(void* data) {
    void** head = data;
    void** next = *(head - 1);
    return (u64)(next - head);
}

static void memory_init(void) {
    ptr = &memory;
    *ptr = ptr;
    ptr = alloc(ptr, 0, 0);
    cache = _list_alloc(sizeof(void*));
    list->init(cache);
}

static void memory_destroy(void) {
    list->destroy(cache);
    _list_free(cache, sizeof(void*));
#ifdef USE_MEMORY_CLEANUP
    cache = 0;
#endif
    _list_free(memory, 3 * sizeof(void*));
    memory = 0;
    ptr = 0;
}

static void* memory_alloc(u64 size) {
    void* tmp = list->pop(cache);
    if (tmp != 0 && offset(tmp) <= size) {
        return tmp;
    }
    ptr = alloc(ptr - 1, ptr, size);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("  0+: 0x%016llx >0x%016llx\n", (u64)ptr, (u64)(*ptr));
#endif
    return ptr;
}

// releases global memory
static void memory_free(void* data) {
    void** head = data;
    void** next = *(head - 1);
    void** last = *(head - 2);
    if (*next == 0) {
        list->push(cache, ptr);
        ptr = last;
        next = ptr + offset(ptr);
        *next = 0;
    }
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
