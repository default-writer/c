#include "internal/list.h"

void list_init(list_context * const ctx);
void list_alloc(list_context * const ctx, abstract_ptr payload);
void list_push(list_context * const ctx, list_ptr* const item);
list_ptr list_pop(list_context * const ctx);
list_ptr list_peek(list_context * const ctx);
void list_free(list_context * const ctx, list_ptr * const item);
void list_destroy(list_context * const ctx);

// default list methods
typedef struct list_vtable {
    // push item on current context (stack)
    void (*push)(list_context * const ctx, list_ptr * const item);
    // pop item on current context (stack)
    list_ptr (*pop)(list_context * const ctx);
    // peek item on current context (stack)
    list_ptr (*peek)(list_context * const ctx);
    // free item on current context (stack)
    void (*free)(list_context * const ctx, list_ptr * const item);
    // alloc item on current context (stack)
    void (*alloc)(list_context * const ctx, abstract_ptr payload);
    // initialize context
    void (*init)(list_context * const ctx);
    // destroy context
    void (*destroy)(list_context * const ctx);
} list_vtable;

// list vtable
static list_vtable list_vt = {
    .alloc = list_alloc,
    .push = list_push,
    .pop = list_pop,
    .peek = list_peek,
    .free = list_free,
    .init = list_init,
    .destroy = list_destroy
};