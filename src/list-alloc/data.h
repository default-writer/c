#ifndef _LIST_EXPERIMENTAL_DATA_H_
#define _LIST_EXPERIMENTAL_DATA_H_

#include "std/common.h"

struct list_data
{
    struct list_alloc_data* data;
    struct list_data* next;
};

struct list_alloc_data {
    /* payload */
    void** data;
    SIZE size;
    struct list_data* next;
};

struct list {
    /* push item on current context (stack) */
    void (*push)(struct list_alloc_data** const current, const void* item);
    /* pop item on current context (stack) */
    const void* (*pop)(struct list_alloc_data** const current);
    /* peek item on current context (stack) */
    const void* (*peek)(struct list_alloc_data** const current);
};

struct list_alloc_data* _new();
void _delete(struct list_alloc_data* ptr);
size_t size();

#endif // _LIST_EXPERIMENTAL_DATA_H_