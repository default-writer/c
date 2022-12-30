#ifndef _LIST_DATA_H_
#define _LIST_DATA_H_

#include "std/common.h"

struct class_methods {
    /* initialize context */
    void (*init)(struct class_methods** const current);
    /* destroy context */
    void (*destroy)(struct class_methods** const current);
    /* returns current class type id */
    LPTR (*get_type)();
};

struct class_methods* _new();
void _delete(struct class_methods* ptr);
size_t size();

#endif // _LIST_DATA_H_
