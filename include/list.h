#include "list-internal.h"

// pointer abstraction on queue/list type with context
typedef struct list_ptr {
    // pointer to queue/list with context
    struct list *ptr;
} list_ptr;

void list_init(list_ptr* const head);
void list_using(list_ptr * const head);
void list_destroy(list_ptr * const head);