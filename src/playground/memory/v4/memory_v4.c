#include "playground/memory/v4/memory_v4.h"

#define MAX_MEMORY 0xffff // 64K bytes

#include "common/alloc.h"

#include "list-micro/data.h"

// the idea:
//
// minimal allocation unit - 64 bit (8-byte block)
//
// allocation layers (32)
//
// layer-0 1 8-byte blocks
// layer-1 2 8-byte blocks
// layer-2 4 8-byte blocks
// layer-3 8 8-byte blocks
// layer-4 16 8-byte blocks
// ...
// layer-n pow(2,n) 8-byte blocks
// ...
// layer-31 2^32 8-byte blocks (32 GB)
//
// practically, it is enough to have 16 levels, 2^16*8 = 524288 bytes
// at worst case, algorithm will lose 50% memory for allocating first 1 MB of data, then, it can't be worse than that
// for a sequence of equal memory block size. i.e.
//
//  blocks - loses
//  1 bytes in 1 byte blocks - 0%
//  2 bytes in 2 byte blocks - 0%
//  3 bytes in 4 byte blocks - 25%
//  4 bytes in 4 byte blocks - 0%
//  5 bytes in 8 byte blocks - 37.5%
//  6 bytes in 8 byte blocks - 25%
//  7 bytes in 8 byte blocks - 12.5%
//  8 bytes in 8 byte blocks - 0%
//
// but if we pass the rest to the cache to direct level below, that will virtually eliminate some space losses, if used
//
// for example, allocating 9 bytes will allocate two 8 byte blocks, by splitting second block to 1 and (1+2+4) block
//
// each layer has own block cache
// if needed, lower layer can allocate in one of upper level blocks depending or number of blocks
//
// when freeing blocks, if 2 sequential blocks forms the bigger block of size for one layer up,
// it will moves to upper level cache
//
// when allocating blocks, if current level blocks cache is empty, we will go to one level up
// after allocation of upper level block, it will move from used on upper level to cached block
// on level below, so it can be utilize by memory allocator on level below allocation level
//
// basically,
// allocating blocks will move down on the allocation level and split (allocated lvl -> cached lvl-1)
// freeing blocks will join the neighbour to the right and move up on the allocation levels (freed lvl -> cached lvl+1)

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

    void** tmp = _list_alloc((size + 2) * sizeof(void*));
    next = *next;
    *next = tmp;

    *tmp = prev;
    *(tmp + 1) = tmp + size + 1;

    current = tmp + 2;

    return current;
}

static u64 offset(void* data) {
    u64 size = 0;
    if (data != 0) {
        void** head = data;
        void** next = *(head - 1);
        size = (u64)(next - head) + 1;
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
    if (tmp != 0 && cached_size >= size) {
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
    list->push(cache, data);
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
    memset(head - 2, 0, (size + 2) * sizeof(void*)); // NOLINT
#endif
    _list_free(head - 2, size);
}

const struct memory_allocator memory_allocator_v4 = {
    .init = memory_init,
    .destroy = memory_destroy,
    .alloc = memory_alloc,
    .free = memory_free
};
