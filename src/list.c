#define DEBUG

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG
// address type (for debugging printf function)
typedef long long unsigned int ADDR;

/* Define a custom `malloc` function. */
void* my_calloc(size_t nmemb, size_t size)
{
    void* ptr = calloc(nmemb, size);
    printf("!alloc: 0x%llx :%ld\n", (ADDR)ptr, size);
    return ptr;
}

/* Define a custom `malloc` function. */
void my_free(void* ptr)
{
    if (ptr != 0) {
        printf("!free: 0x%llx\n", (ADDR)ptr);
    }
    free(ptr);
}

/* Override the default `malloc` function used by Rexo with ours. */
#define _LIST_CALLOC my_calloc

/* Override the default `free` function used by Rexo with ours. */
#define _LIST_FREE my_free

#endif

#ifndef _LIST_CALLOC
    #include <stdlib.h>
    #define _LIST_CALLOC calloc
#endif

#ifndef _LIST_FREE
    #include <stdlib.h>
    #define _LIST_FREE free
#endif

#define ALLOC(size, type) (type*)_LIST_CALLOC(1, sizeof(type))
#define FREE(ptr) _LIST_FREE(ptr)

#include "list.h"

// initializes the new context's root element
// as a result, new memory block will be allocated
// current context pointer set to zero
void list_init(struct list_context* const ctx) {
    // sets current context's root element
    ctx->root = ctx->head = ALLOC(1, struct list);
    // sets current context's counter to zero
    ctx->count = 0;
}

// allocates a memory for provided payload 
// at current context, data payload stored at allocated memory buffer
// as a result, items counter will increase
void list_alloc(struct list_context* const ctx, abstract_ptr payload) {
    // get current context's head
    struct list* head = ctx->head;
    // stores into pre-allocated value newly allocated memory buffer pointer
    struct list* tmp = ALLOC(1, struct list);
    // sets the new data into allocated memory buffer
    tmp->payload = payload;
    // pushes new item on top of the stack in current context
    list_push(ctx, &tmp);

}

// push new item to existing context
// at current context, new item will be added as next element
// for the new item, add current head as previous element
// as a result, head will advances to new position, represented as new item
struct list* list_push(struct list_context* const ctx, struct list** const item) {
    if (item == 0 || *item == 0 || *item == list_ptr_null) {
        return list_ptr_null;
    }
    // get current context's head
    struct list* head = ctx->head;
    // assign item pointer to head's next pointer value
    head->next = *item;
    // assign item's prev pointer to head pointer
    (*item)->prev = ctx->head;
    // advances position of head pointer to the new head
    ctx->head = *item;
    // increment current context's counter by one
    ctx->count++;    
    // return previous context's head
    return head;
}

// pop existing element at the top of the stack/queue/list
// at current context, existing head will be removed out of stack
// for the new stack header, correcponding values will be fixed
// as a result, header will be rewinded to previous position, represented as head's reference to previos head
struct list* list_pop(struct list_context* const ctx) {
    // get current context's head
    struct list* head = ctx->head;
    // if we call method on empty stack, do not return root element, return null element by convention
    if (head == 0 || head->prev == 0) {
        // returns default element as null element
        return list_ptr_null;
    }
    // gets previos pointer
    struct list* prev = head->prev;
    prev->next = list_ptr_null;
    // otherwize, assign current stack head pointer to temporary
    // gets temporary pointer value
    struct list* ptr = head;
    // detouches the pointer from the list
    ptr->prev = list_ptr_null;
    ptr->next = list_ptr_null;
    // rewinds head pointer to previous pointer value
    ctx->head = prev;
    // decrement current context counter
    ctx->count--;
    // returns removed element
    return ptr;
}

// peek existing element at the top of the stack/queue/list
// at current context, existing head
struct list* list_peek(struct list_context* const ctx) {
    // get current context's head
    struct list* head = ctx->head;
    struct list* tmp = head;
    // if we call method on empty stack, do not return root element, return null element by convention
    if (head == 0 || head->prev == 0) {
        // returns default element as null element
        return list_ptr_null;
    }
    // otherwize, assign current stack head pointer to temporary
    tmp = head;
    // returns head element
    return tmp;
}

// get root element
// at current context, existing tail
struct list* list_root(struct list_context* const ctx) {
    // get current context's tail
    struct list* root = ctx->root;
    // if we call method on empty stack, do not return root element, return null element by convention
    if (root == 0 || root->next == 0) {
        // returns default element as null element
        return list_ptr_null;
    }
    // otherwize, assign current stack head pointer to temporary
    struct list* tmp = root->next;
    // returns head element
    return tmp;
}
// frees up memory assigned for allocation of items at current position
// at current context, all data needed to be claimed, will be freed
// as a result, all items, starting from specified item, will be deleted
void list_free(struct list_context* const ctx, struct list** const item) {
    // assign currently selected item pointer to temporary
    struct list* tmp = *item;
    // until we run out of stack or stop at root element
    while (tmp != 0) {
        // gets temporary pointer value
        struct list* ptr = tmp;
        struct list* next = tmp->next;
        // zero all pointers
        ptr->prev = list_ptr_null;
        ptr->next = list_ptr_null;
        ptr->payload = 0;
        // free temporary pointer value
        FREE(ptr);
        // advances temporary pointer value to the next item
        tmp = next;
    }
    // all stack items are processed
    *item = list_ptr_null;
}

// destroys the memory stack
// frees all memory elements
// as a result, memory will be freed
void list_destroy(struct list_context* const ctx) {
    // get current context's head
    list_free(ctx, &ctx->root);
    // all stack items are processed
}
