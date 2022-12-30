#ifndef _LIST_DATA_H_
#define _LIST_DATA_H_

#include "std/common.h"

struct data;

struct class {
    /* private data definition */
    struct data* data;
    /* reads data */
    void* (*get_data)(const struct class *class);
    /* writes data */
    void* (*set_data)(const struct class *class, void* data);
    /* returns current class type id */
    LPTR (*get_type)(const struct class *class);
};

struct class* _new();
void _delete(struct class* ptr);
size_t size();

#endif // _LIST_DATA_H_
