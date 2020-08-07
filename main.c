#define DEBUG

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

#ifdef DEBUG
// address type (for debugging printf function)
typedef long long unsigned int ADDR;
#endif

// default list usage scenario
void using_list(void (*list_using)(q_type_context * const)) {
    // initialize current context (stack)
    q_type_context context;
    // initialize list
    list_init(&context);
    // call user method
    list_using(&context);
    // destroy list
    list_destroy(&context);
}

// print item on current context (stack)
void print_item(q_type_context * const ctx) {
    // get current context's head
    q_type_ptr* head = &(ctx->head);
    // gets pre-allocated (compiler-generated) stack value as temporary
    q_type_ptr tmp;
#ifdef DEBUG
    // visualise item
    printf("alloc: 0x%llx 0x%llx\n", (ADDR)tmp.ptr, (ADDR)tmp.ptr->payload);
#endif
}

// print all stack trace to output
// in a single loop, print out all ements except root element (which does not have a payload)
// as a result, all stack will be printed in last-to-first order (reverse)
void list_print(q_type_context * const ctx) {
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

// use list
void list_using(q_type_context* const ctx) {
    // access context's functions pointer
    list* list = ctx->f.ptr;
    abstract_ptr payload = (abstract_ptr)0xdeadbeef;
    list->alloc(ctx, payload);
    print_item(ctx);
    list->alloc(ctx, ++payload);
    print_item(ctx);
    list->alloc(ctx, ++payload);
    print_item(ctx);
    list->alloc(ctx, ++payload);
    print_item(ctx);
    list->alloc(ctx, ++payload);
    print_item(ctx);
#ifdef DEBUG
    printf("\n");
#endif
#ifdef DEBUG
    list_print(ctx);
#endif
    q_type_ptr q_pop0 = list_pop(ctx); 
    list_free(ctx, &q_pop0);
#ifdef DEBUG
    list_print(ctx);
#endif
    q_type_ptr q_pop1 = list_pop(ctx); 
    list_free(ctx, &q_pop1);
#ifdef DEBUG
    list_print(ctx);
#endif
    q_type_ptr q_pop2 = list_pop(ctx); 
    list_free(ctx, &q_pop2);
#ifdef DEBUG
    list_print(ctx);
#endif
    q_type_ptr q_pop3 = list_pop(ctx); 
    list_push(ctx, &q_pop3);
    q_pop3 = list_pop(ctx); 
    list_free(ctx, &q_pop3);
#ifdef DEBUG
    list_print(ctx);
#endif
    q_type_ptr q_pop4 = list_pop(ctx); 
    list_free(ctx, &q_pop4);
#ifdef DEBUG
    list_print(ctx);
#endif
    q_type_ptr q_pop5 = list_pop(ctx); 
    list_free(ctx, &q_pop5);
#ifdef DEBUG
    list_print(ctx);
#endif
}

int main() {
    using_list(list_using);
}