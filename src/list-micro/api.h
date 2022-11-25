#ifndef _LIST_MICRO_API_H_
#define _LIST_MICRO_API_H_

#include "globals.h"
#include "alloc.h"
#include "data.h"

/* queue/list: vtable definition */
struct list_class {
    /* push item on current context (stack) */
    void (*push)(struct list** const current, void* item);
    /* pop item on current context (stack) */
    void* (*pop)(struct list** const current);
};

struct list_vtable
{
    /* initialize context */
    void (*init)(struct list** const current);
    /* destroy context */
    void (*destroy)(struct list** const current);
    struct list_class* self;
};

#endif // _LIST_MICRO_API_H_