# readme

```c++

#include <stdlib.h>
#include "list/api.h"

/* queue/list context */
struct memory_manager_context { 
    /* alloc buffer list */
    struct list* alloc;
    /* free buffer list */
    struct list* free;
    /* available */
    size_t available;
    /* free */
    size_t used;
};

/* queue/list: vtable definition0 */
struct memory_manager_vtable {
    /* initialize context */
    void (*init)(struct memory_manager_context* const ctx);
    /* returns next memory pointer */
    struct list* (*alloc)(struct memory_manager_context* const ctx, size_t nmemb, size_t size);
    /* releases memory pointer */
    void (*free)(struct memory_manager_context* const ctx, struct list** const pointer);
    /* destroy context */
    void (*destroy)(struct memory_manager_context* const ctx);
};

/*
Memory manager: Type 1
1) Should "alloc" virtually, and return list items, not pointers to arbitrary memory

Method name: alloc

Structure: alloc_buffer ==> list

Structure: alloc_ptr
_____________
| ptr       | - pointer to allocated buffer
|-----------|
| allocated | - number of allocated bytes
|___________|

why do we need "allocated on the client", is because we will merge with buffer to the right 

allocation:

we have pointer to allocated initial buffer length 1000, and list contains 1 record
we request 10 bytes to allocated
instead of allocation from OS, we cut first list element greater than allocation request
into two items,

before: [ list item 1 | 10000 ]
after: [ list item 1 | 9990 ]->[ list item 2 | 10 ]

so *(list1 + 10000) - is the next byte to the last allocated byte
and *(list2 + 10) == * (list1 + 10000), so allocated value of first element in a list
starts to decrease.

Easy to prove that we can easily track of list available chunks, 
we will do not make any memory movements (relocations) at all, to sort optimize, cause
we know that this is a very very very bad idea, i.e. one thread to GC, another to MM!

this is crazy and will not work in single-threaded environment well.

Instead, we will try to place blocks sorted from right to left from smallest to 
biggest

Picture I.

->all
______________________
|1    |xxx|2 |x|3  |x|
----------------------

->free
_________________
|1    ||2 ||3  ||
-----------------

->allocated
____________
||xxx||x||x|
------------

what we need to do is to think about

a) how do we know the size of allocated/freed blocks? we pass it to outer system
b) how we can get situation at picture (I)?

alloc 1 -> ptr1
alloc 3 -> ptr2
alloc 1 -> ptr3
alloc 2 -> ptr4
alloc 3 -> ptr5

free ptr2
free ptr4

c)should we compact or relocate memory? Nope if we got 4 Terabytes, and yes 
if we got calculator for 4K bytes of free memory

allocation (using double stack):

1.1) moving from the head of list of free blocks towards the root, find one with enough space
1.2) decrease allocated counter for that block (we do not need allocated counter, cause it is a ptr - ptr.prev.ptr), link it to newly created block 
1.3) add newly created allocation block to the list of allocated blocks record 

destroying (freeing):

1.1) moving from the head of list allocated blocks towards the root, find one matches the pointer
1.2) insert allocation block back to the list of free blocks or it there is a free block to the left,
1.3) increase counter of free block to the left (we do not need counter cause it is ptr - ptr.pre.ptr), delete block from the list of allocated blocks

2) Should be able to "free" memory pointer accordingly
*/

#define DEBUG

#define MEMORY_MANAGER

/* #define DIRTY */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG
/* address type (for debugging printf function) */
typedef long long unsigned int ADDR;
#endif

#ifdef MEMORY_MANAGER

/* Define a custom `malloc` function. */
void* _calloc(size_t nmemb, size_t size)
{
    void* ptr = calloc(nmemb, size);
#ifdef DEBUG
    printf("!alloc: 0x%llx :%ld\n", (ADDR)ptr, size);
#endif
    return ptr;
}

/* Define a custom `free` function. */
void _free(void* ptr)
{
    if (ptr != 0) {
#ifdef DEBUG
        printf("!free: 0x%llx\n", (ADDR)ptr);
#endif
    }
    free(ptr);
}

/* Override the default `malloc` function used by Rexo with ours. */
#define _MEMORY_MANAGER_CALLOC _calloc

/* Override the default `free` function used by Rexo with ours. */
#define _MEMORY_MANAGER_FREE _free

#endif //MEMORY_MANAGER

#ifndef _MEMORY_MANAGER_CALLOC
    #include <stdlib.h>
    #define _MEMORY_MANAGER_CALLOC calloc
#endif

#ifndef _MEMORY_MANAGER_FREE
    #include <stdlib.h>
    #define _MEMORY_MANAGER_FREE free
#endif

#define ALLOC(size, type) (type*)_MEMORY_MANAGER_CALLOC(1, sizeof(type))
#define FREE(ptr) _MEMORY_MANAGER_FREE(ptr)

#include "memory-manager.h"

/* default memory manager methods */
void memory_manager_init(struct memory_manager_context* const ctx);
struct list* memory_manager_alloc(struct memory_manager_context* const ctx, size_t nmemb, size_t size);
void memory_manager_free(struct memory_manager_context* const ctx, struct list** const pointer);
void memory_manager_destroy(struct memory_manager_context* const ctx);

/* list vtable */
const struct memory_manager_vtable memory_manager_vt = {
    .alloc = memory_manager_alloc,
    .free = memory_manager_free,
    .init = memory_manager_init,
    .destroy = memory_manager_destroy
};

/* initializes the new context's root element */
/* as a result, new memory block will be allocated */
/* current context pointer set to zero */
void memory_manager_init(struct memory_manager_context* const ctx) {
    ctx->alloc = ALLOC(1, struct list);
    ctx->free = ALLOC(1, struct list);
    ctx->free = 1024;
}


/* returns next memory pointer */
struct list* memory_manager_alloc(struct memory_manager_context* const ctx, size_t nmemb, size_t size) {
    /* TODO: implement memory manager alloc */
    return (struct list*)0;
}

/* releases memory pointer */
void memory_manager_free(struct memory_manager_context* const ctx, struct list** const pointer) {
    /* TODO: implement memory manager free */
}

void memory_manager_destroy(struct memory_manager_context* const ctx) {
    FREE(ctx->alloc);
    FREE(ctx->free);
}
```
