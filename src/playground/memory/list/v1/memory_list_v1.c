#include "memory_list_v1.h"
#include "common/alloc.h"
#include "list-micro/data.h"

/* the idea: */
//
/* minimal allocation unit - 64 bit (8-byte block) */
//
/* allocation layers (32) */
//
/* layer-0 1 8-byte blocks */
/* layer-1 2 8-byte blocks */
/* layer-2 4 8-byte blocks */
/* layer-3 8 8-byte blocks */
/* layer-4 16 8-byte blocks */
/* ... */
/* layer-n pow(2,n) 8-byte blocks */
/* ... */
/* layer-31 2^32 8-byte blocks (32 GB) */
//
/* practically, it is enough to have 16 levels, 2^16*8 = 524288 bytes */
/* at worst case, algorithm will lose 50% memory for allocating first 1 MB of data, then, it can't be worse than that */
/* for a sequence of equal memory block size. i.e. */
//
/*  blocks - loses */
/*  1 bytes in 1 byte blocks --- 0% */
/*  2 bytes in 2 byte blocks --- 0% */
/*  3 bytes in 4 byte blocks --- 25% */
/*  4 bytes in 4 byte blocks --- 0% */
/*  5 bytes in 8 byte blocks --- 37.5% */
/*  6 bytes in 8 byte blocks --- 25% */
/*  7 bytes in 8 byte blocks --- 12.5% */
/*  8 bytes in 8 byte blocks --- 0% */
//
/* but if we pass the rest to the cache to direct level below, that will virtually eliminate some space losses, if used */
//
/* for example, allocating 9 bytes will allocate two 8 byte blocks, by splitting second block to 1 and (1+2+4) block */
//
/* each layer has own block cache */
/* if needed, lower layer can allocate in one of upper level blocks depending or number of blocks */
//
/* when freeing blocks, if 2 sequential blocks forms the bigger block of size for one layer up, */
/* it will moves to upper level cache */
//
/* when allocating blocks, if current level blocks cache is empty, we will go to one level up */
/* after allocation of upper level block, it will move from used on upper level to cached block */
/* on level below, so it can be utilize by memory allocator on level below allocation level */
//
/* basically, */
/* allocating blocks will move down on the allocation level and split (allocated lvl -> cached lvl-1) */
/* freeing blocks will join the neighbour to the right and move up on the levels (freed lvl -> cached lvl+1) */

/* list definition */
extern const struct list list_micro_definition;

/* list definition */
static const struct list* list = &list_micro_definition;
static struct list_data** cache;

void memory_list_init(void) {
    cache = _list_alloc(sizeof(void*));
    list->init(cache);
}

void* memory_list_peek(void) {
    /* struct memory_ref* ptr */
    /* void* data = _ptr(ptr); */
    /* return data; */
    return list->peek(cache);
}

void memory_list_push(void* data) {
    /* struct memory_ref* next = _ref(data); */
    /* if (ptr != 0) { */
    /*     struct memory_ref* prev = ptr->prev; */
    /*     prev->next = next->prev; */
    /* } */
    list->push(cache, data);
}

void* memory_list_pop(void) {
    /* void* data = _ptr(ptr); */
    /* if (ptr != 0) { */
    /*     struct memory_ref* next = ptr->next; */
    /*     struct memory_ref* prev = ptr->prev; */
    /*     if (next != 0) { */
    /*         next->prev = prev; */
    /*     } */
    /*     if (prev != 0) { */
    /*         prev->next = next; */
    /*     } */
    /* } */
    /* return data; */
    return list->pop(cache);
}

void memory_list_destroy(void) {
    list->destroy(cache);
    _list_free(cache, sizeof(void*));
#ifdef USE_MEMORY_CLEANUP
    cache = 0;
#endif
}
