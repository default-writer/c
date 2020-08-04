#include "internal/list.h"

// queue/list: current context structure, and methods to operate on it
typedef struct list {
    // current context (stack)
    q_type_context context;
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

// pointer abstraction on queue/list type with context
typedef struct list_ptr {
    // pointer to queue/list with context
    struct list *ptr;
} list_ptr;

void list_init(list_ptr* const head);
void list_using(list_ptr * const head);
void list_destroy(list_ptr * const head);