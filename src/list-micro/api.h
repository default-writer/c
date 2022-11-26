#ifndef _LIST_MICRO_API_H_
#define _LIST_MICRO_API_H_

#include "globals.h"
#include "alloc.h"

/* queue/list: vtable definition */
struct list {
    /* points to previous node */
    struct list* prev;
    /* payload */
    void*  payload;
    /* push item on current context (stack) */
    void (*push)(struct list** const current, void* item);
    /* pop item on current context (stack) */
    void* (*pop)(struct list** const current);
    /* initialize context */
    void (*init)(struct list** const current);
    /* destroy context */
    void (*destroy)(struct list** const current);
    /* link to self-contained methods structure */
    const struct list* self;
};

extern const struct list list_definition;

#endif // _LIST_MICRO_API_H_