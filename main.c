#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#define DEBUG
// address type (for debugging printf function)
typedef long long unsigned int ADDR;

// static default implementation of null value for queue/struct
const static const q_type_ptr const q_type_ptr_null;

// push new item to existing context
// at current context, new item will be added as next element
// for the new item, add current head (points to top of the stack/queue/list) as previous element
// as a result, head will advances to new position, represented as new item
void q_type_push(q_type_context * const ctx, q_type_ptr* const item) {
    // get current context's head
    q_type_ptr* head = &(ctx->head);
    // assign item pointer to head's next pointer value
    head->ptr->next.ptr = item->ptr;
    // assign item's prev pointer to head pointer
    item->ptr->prev.ptr = head->ptr;
    // advances position of head pointer to the new head
    head->ptr = item->ptr;
}

// pop existing element at the top of the stack/queue/list
// at current context, existing head (points to top of the stack/queue/list) will be removed out of stack
// for the new stach header, correcponding values will be fixed
// as a result, header will be rewinded to previous position, represented as head's reference to previos head
q_type_ptr q_type_pop(q_type_context * const ctx) {
    // get current context's head
    q_type_ptr* head = &(ctx->head);
    // gets pre-allocated (compiler-generated) stack value as temporary
    q_type_ptr tmp;
    // if we call method on empty stack, do not return root element, return null element by convention
    if (head->ptr->prev.ptr == 0) {
        // returns default element as null element
        return q_type_ptr_null;
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

// print all stack trace to output
// in a single loop, print out all ements except root element (which does not have a payload)
// as a result, all stack will be printed in last-to-first order (reverse)
void q_type_print(q_type_context * const ctx) {
    // get current context's head
    q_type_ptr* head = &(ctx->head);
    // sets the counter
    int i = 0;
    // gets pre-allocated (compiler-generated) stack value as temporary
    q_type_ptr tmp;
    // assigns current's head pointer to the temporary
    tmp.ptr = head->ptr;
    // until we found root element (element with no previous element reference)
    while (tmp.ptr->prev.ptr != 0) {
#ifdef DEBUG
        // debug output of memory dump
        printf("%d: 0x%llx 0x%llx\n", ++i, (ADDR)tmp.ptr, (ADDR)tmp.ptr->payload);
#endif
        // remember temprary's prior pointer value to temporary
        tmp.ptr = tmp.ptr->prev.ptr;
    }
    // stop on root element
#ifdef DEBUG
    // visualise loop break 
    printf("\n");
#endif
}

// frees up memory assigned for allocation of items at current position
// at current context, all data needed to be claimed, will be freed
// as a result, all items, starting from specified item, will be deleted
void q_type_free(q_type_context * const ctx, q_type_ptr * const item) {
    // get current context's head
    q_type_ptr* head = &(ctx->head);
    // gets pre-allocated (compiler-generated) stack value as temporary
    q_type_ptr tmp;
    // assign currently selected item pointer to temporary
    tmp.ptr = item->ptr;
    // until we run out of stack or stop at root element
    while (ctx->count > 0 && tmp.ptr != 0) {
#ifdef DEBUG
        // debug output of memory dump
        printf("free: 0x%llx 0x%llx\n", (ADDR)tmp.ptr, (ADDR)tmp.ptr->payload);
#endif
        // gets temporary pointer value
        q_type* ptr = tmp.ptr;
        // advances temporary pointer value to the next item
        tmp.ptr = tmp.ptr->next.ptr;
        // free temporary pointer value
        free(ptr);
        // decrement current context counter
        ctx->count--;
    }
    // all stack items are processed
#ifdef DEBUG
    // visualise loop break 
    printf("\n");
#endif
}

// allocates a memory for provided payload 
// at current context, data payload stored at allocated memory buffer
// as a result, items counter will increase
void q_type_alloc(q_type_context * const ctx, abstract_ptr payload) {
    // get current context's head
    q_type_ptr* head = &(ctx->head);
    // gets pre-allocated (compiler-generated) stack value as temporary
    q_type_ptr tmp;
    // stores into pre-allocated value newly allocated memory buffer pointer
    tmp.ptr = (q_type*)malloc(sizeof(q_type));
    // sets the new data into allocated memory buffer
    tmp.ptr->payload = payload;
#ifdef DEBUG
    // visualise loop break
    printf("alloc: 0x%llx 0x%llx\n", (ADDR)tmp.ptr, (ADDR)tmp.ptr->payload);
#endif
    // pushes new item on top of the stack in current context
    q_type_push(ctx, &tmp);
    // increment current context's counter by one
    ctx->count++;
}

// initializes the new context's root element
// as a result, new memory block will be allocated
// current context pointer set to zero
void q_type_init(q_type_context * const ctx) {
    // stores into pre-allocated value newly allocated memory buffer pointer
    ctx->head.ptr = (q_type*)malloc(sizeof(q_type));
    // sets current context's counter to zero
    ctx->count = 0;
}

// destroys the memory stack
// frees all memory elements
// as a result, memory will be freed
void q_type_destroy(q_type_context * const ctx) {
    // get current context's head
    q_type_ptr* head = &(ctx->head);
    // gets pre-allocated (compiler-generated) stack value as temporary
    q_type_ptr tmp;
    // assign currently selected item pointer to temporary
    tmp.ptr = head->ptr;
    // until we run out of stack or stop at root element
    while (ctx->count > 0 && tmp.ptr != 0) {
#ifdef DEBUG
        // debug output of memory dump
        printf("free: 0x%llx 0x%llx\n", (ADDR)tmp.ptr, (ADDR)tmp.ptr->payload);
#endif
        // gets temporary pointer value
        q_type* ptr = tmp.ptr;
        // advances temporary pointer value to the previous value
        tmp.ptr = tmp.ptr->prev.ptr;
        // free temporary pointer value
        free(ptr);
        // decrements current context's counter
        ctx->count--;
    }
    // all stack items are processed
#ifdef DEBUG
    // visualise loop break 
    printf("\n");
#endif
}

typedef struct list_ptr {
    struct list *ptr;
} list_ptr;

typedef struct list {
    q_type_context context;
    void (*push)(q_type_context * const ctx, q_type_ptr * const item);
    q_type_ptr (*pop)(q_type_context * const ctx);
    void (*print)(q_type_context * const ctx);
    void (*free)(q_type_context * const ctx, q_type_ptr * const item);
    void (*alloc)(q_type_context * const ctx, abstract_ptr payload);
} list;

void list_q_type_push(list_ptr * const head, q_type_ptr* const item) {
    head->ptr->push(&(head->ptr->context), item);
}

q_type_ptr list_q_type_pop(list_ptr * const head) {
    return head->ptr->pop(&(head->ptr->context));
}

void list_q_type_print(list_ptr * const head) {
    head->ptr->print(&(head->ptr->context));
}

void list_q_type_free(list_ptr * const head, q_type_ptr * const item) {
    head->ptr->free(&(head->ptr->context), item);
}

void list_q_type_alloc(list_ptr * const head, abstract_ptr payload) {
    head->ptr->alloc(&(head->ptr->context), payload);
}

void list_init(list_ptr* const head) {
    head->ptr = (list*)malloc(sizeof(list));
    head->ptr->alloc = q_type_alloc;
    head->ptr->push = q_type_push;
    head->ptr->pop = q_type_pop;
    head->ptr->print = q_type_print;
    head->ptr->free = q_type_free;
    q_type_init(&(head->ptr->context));
}

void list_destroy(list_ptr * const head) {
    q_type_destroy(&(head->ptr->context));
    free(head->ptr);
}

void list_using(list_ptr * const head) {
    abstract_ptr payload = (abstract_ptr)0xdeadbeef;
    list_q_type_alloc(head, payload);
    list_q_type_alloc(head, ++payload);
    list_q_type_alloc(head, ++payload);
    list_q_type_alloc(head, ++payload);
    list_q_type_alloc(head, ++payload);
#ifdef DEBUG
    printf("\n");
#endif
#ifdef DEBUG
    list_q_type_print(head);
#endif
    q_type_ptr q_pop0 = list_q_type_pop(head); 
    list_q_type_free(head, &q_pop0);
#ifdef DEBUG
    list_q_type_print(head);
#endif
    q_type_ptr q_pop1 = list_q_type_pop(head); 
    list_q_type_free(head, &q_pop1);
#ifdef DEBUG
    list_q_type_print(head);
#endif
    q_type_ptr q_pop2 = list_q_type_pop(head); 
    list_q_type_free(head, &q_pop2);
#ifdef DEBUG
    list_q_type_print(head);
#endif
    q_type_ptr q_pop3 = list_q_type_pop(head); 
    list_q_type_push(head, &q_pop3);
    q_pop3 = list_q_type_pop(head); 
    list_q_type_free(head, &q_pop3);
#ifdef DEBUG
    list_q_type_print(head);
#endif
    q_type_ptr q_pop4 = list_q_type_pop(head); 
    list_q_type_free(head, &q_pop4);
#ifdef DEBUG
    list_q_type_print(head);
#endif
    q_type_ptr q_pop5 = list_q_type_pop(head); 
    list_q_type_free(head, &q_pop5);
#ifdef DEBUG
    list_q_type_print(head);
#endif
}

void using_list() {
    list_ptr head;
    list_init(&head);
    list_using(&head);
    list_destroy(&head);
}

int main() {
    using_list();
}