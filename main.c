#include <stdio.h>
#include <stdlib.h>
#include "list.h"

#define DEBUG

void using_list(void (*list_using)(q_type_context * const)) {
    // current context (stack)
    q_type_context context;
    list_init(&context);
    list_using(&context);
    list_destroy(&context);
}

// use list
void list_using(q_type_context* const ctx) {
    list* list_methods = ctx->f.ptr;
    abstract_ptr payload = (abstract_ptr)0xdeadbeef;
    list_alloc(ctx, payload);
    list_alloc(ctx, ++payload);
    list_alloc(ctx, ++payload);
    list_alloc(ctx, ++payload);
    list_alloc(ctx, ++payload);
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