#include "internal/list.h"

// queue/list: current context structure, and methods to operate on it
typedef struct list {
    // push item on current context (stack)
    void (*push)(q_type_context * const ctx, q_type_ptr * const item);
    // poph item on current context (stack)
    q_type_ptr (*pop)(q_type_context * const ctx);
    // print item on current context (stack)
    void (*print)(q_type_context * const ctx);
    // free item on current context (stack)
    void (*free)(q_type_context * const ctx, q_type_ptr * const item);
    // alloc item on current context (stack)
    void (*alloc)(q_type_context * const ctx, abstract_ptr payload);
} list;


void list_init(q_type_context* const ctx);
void list_using(q_type_context* const ctx);
void list_destroy(q_type_context* const ctx);

void list_q_type_push(q_type_context* const ctx, q_type_ptr* const item);
q_type_ptr list_q_type_pop(q_type_context* const ctx);
void list_q_type_print(q_type_context* const ctx);
void list_q_type_free(q_type_context* const ctx, q_type_ptr * const item);
void list_q_type_alloc(q_type_context* const ctx, abstract_ptr payload);
