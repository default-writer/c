#ifndef _LIST_MICRO_API_H_
#define _LIST_MICRO_API_H_

#include "globals.h"
#include "typedefs.h"
#include "alloc.h"

struct list_data;

/* queue/list: vtable definition */
struct list {
    /* push item on current context (stack) */
    void (*push)(struct list_data** const current, void* item);
    /* pop item on current context (stack) */
    void* (*pop)(struct list_data** const current);
    /* initialize context */
    void (*init)(struct list_data** const current);
    /* destroy context */
    void (*destroy)(struct list_data** const current);
};

struct list_class {
    /* link to self-contained methods structure */
    const struct list* self;
};

#ifdef DEBUG_ALLOC
void list_print_head(struct list_data** const current);
void list_print(struct list_data** const current);
#endif

extern const struct list_class list_definition;

#endif // _LIST_MICRO_API_H_