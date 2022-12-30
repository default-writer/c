#ifndef _LIST_DATA_H_
#define _LIST_DATA_H_

#include "std/common.h"

struct class {
    /* initialize context */
    void (*init)(struct class** const current);
    /* destroy context */
    void (*destroy)(struct class** const current);
    /* returns current class type id */
    LPTR (*get_type)();
};

struct class* _new();
void _delete(struct class* ptr);
size_t size();

#endif // _LIST_DATA_H_
