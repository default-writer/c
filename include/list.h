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

/* list constructor/desctructor */

// default list constructor
void list_init(q_type_context* const ctx);
// default list destructor
void list_destroy(q_type_context* const ctx);

/* default list methods */

// push item to the top of stack
void list_push(q_type_context* const ctx, q_type_ptr* const item);
// pop item from the top of stack
q_type_ptr list_pop(q_type_context* const ctx);
// print stack
void list_print(q_type_context* const ctx);
// free memory used by stack item
void list_free(q_type_context* const ctx, q_type_ptr * const item);
// allcates memory used by stack item
void list_alloc(q_type_context* const ctx, abstract_ptr payload);
