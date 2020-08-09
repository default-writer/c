#include "list.h"

// static default implementation of null value for queue/struct
const static const list_ptr list_ptr_null;

// push new item to existing context
// at current context, new item will be added as next element
// for the new item, add current head as previous element
// as a result, head will advances to new position, represented as new item
void list_push(list_context * const ctx, list_ptr* const item) {
    // get current context's head
    list_ptr* head = &(ctx->head);
    // assign item pointer to head's next pointer value
    head->ptr->next.ptr = item->ptr;
    // assign item's prev pointer to head pointer
    item->ptr->prev.ptr = head->ptr;
    // advances position of head pointer to the new head
    head->ptr = item->ptr;
}

// pop existing element at the top of the stack/queue/list
// at current context, existing head will be removed out of stack
// for the new stach header, correcponding values will be fixed
// as a result, header will be rewinded to previous position, represented as head's reference to previos head
list_ptr list_pop(list_context * const ctx) {
    // get current context's head
    list_ptr* head = &(ctx->head);
    // gets pre-allocated stack value as temporary
    list_ptr tmp;
    // if we call method on empty stack, do not return root element, return null element by convention
    if (head->ptr->prev.ptr == 0) {
        // returns default element as null element
        return list_ptr_null;
    }
    // otherwize, assign current stack head pointer to temporary
    tmp.ptr = head->ptr;
    // rewinds head pointer to previous pointer value
    head->ptr = tmp.ptr->prev.ptr;
    // clears not valid forward link to element to be removed
    head->ptr->next.ptr = 0;
    // returns removed element
    return tmp;
}

// peek existing element at the top of the stack/queue/list
// at current context, existing head
list_ptr list_peek(list_context * const ctx) {
    // get current context's head
    list_ptr* head = &(ctx->head);
    // gets pre-allocated stack value as temporary
    list_ptr tmp;
    // if we call method on empty stack, do not return root element, return null element by convention
    if (head->ptr->prev.ptr == 0) {
        // returns default element as null element
        return list_ptr_null;
    }
    // otherwize, assign current stack head pointer to temporary
    tmp.ptr = head->ptr;
    // returns head element
    return tmp;
}

// frees up memory assigned for allocation of items at current position
// at current context, all data needed to be claimed, will be freed
// as a result, all items, starting from specified item, will be deleted
void list_free(list_context * const ctx, list_ptr * const item) {
    // get current context's head
    list_ptr* head = &(ctx->head);
    // gets pre-allocated stack value as temporary
    list_ptr tmp;
    // assign currently selected item pointer to temporary
    tmp.ptr = item->ptr;
    // until we run out of stack or stop at root element
    while (ctx->count > 0 && tmp.ptr != 0) {
        // gets temporary pointer value
        list* ptr = tmp.ptr;
        // advances temporary pointer value to the next item
        tmp.ptr = tmp.ptr->next.ptr;
        // free temporary pointer value
        free(ptr);
        // decrement current context counter
        ctx->count--;
    }
    // all stack items are processed
}

// allocates a memory for provided payload 
// at current context, data payload stored at allocated memory buffer
// as a result, items counter will increase
void list_alloc(list_context * const ctx, abstract_ptr payload) {
    // get current context's head
    list_ptr* head = &(ctx->head);
    // gets pre-allocated stack value as temporary
    list_ptr tmp;
    // stores into pre-allocated value newly allocated memory buffer pointer
    tmp.ptr = (list*)malloc(sizeof(list));
    // sets the new data into allocated memory buffer
    tmp.ptr->payload = payload;
    // pushes new item on top of the stack in current context
    list_push(ctx, &tmp);
    // increment current context's counter by one
    ctx->count++;
}

// initializes the new context's root element
// as a result, new memory block will be allocated
// current context pointer set to zero
void list_init(list_context * const ctx) {
    // stores into pre-allocated value newly allocated memory buffer pointer
    ctx->head.ptr = (list*)malloc(sizeof(list));
    // sets current context's counter to zero
    ctx->count = 0;
    // sets the root element 
    ctx->root.ptr = ctx->head.ptr;
}

// destroys the memory stack
// frees all memory elements
// as a result, memory will be freed
void list_destroy(list_context * const ctx) {
    // get current context's head
    list_ptr* head = &(ctx->head);
    // gets pre-allocated stack value as temporary
    list_ptr tmp;
    // assign currently selected item pointer to temporary
    tmp.ptr = head->ptr;
    // until we run out of stack or stop at root element
    while (ctx->count > 0 && tmp.ptr != 0) {
        // gets temporary pointer value
        list* ptr = tmp.ptr;
        // advances temporary pointer value to the previous value
        tmp.ptr = tmp.ptr->prev.ptr;
        // free temporary pointer value
        free(ptr);
        // decrements current context's counter
        ctx->count--;
    }
    // all stack items are processed
}
