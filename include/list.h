#include "internal/list.h"

// default list methods
typedef struct list {
    // push item on current context (stack)
    void (*push)(q_type_context * const ctx, q_type_ptr * const item);
    // pop item on current context (stack)
    q_type_ptr (*pop)(q_type_context * const ctx);
    // peek item on current context (stack)
    q_type_ptr (*peek)(q_type_context * const ctx);
    // free item on current context (stack)
    void (*free)(q_type_context * const ctx, q_type_ptr * const item);
    // alloc item on current context (stack)
    void (*alloc)(q_type_context * const ctx, abstract_ptr payload);
} list;

// default list constructor
void list_create(q_type_context* const ctx);
// default list destructor
void list_destroy(q_type_context* const ctx);

