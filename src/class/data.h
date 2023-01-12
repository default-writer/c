#ifndef _LIST_DATA_H_
#define _LIST_DATA_H_

#include "std/common.h"

struct data;

struct class {
    /* private data definition */
    struct data* data;
    /* reads data */
    const void* (*get_data)(const struct class *class);
    /* writes data */
    void (*set_data)(const struct class *class, const void* data);
    /* returns current class type id */
    LPTR (*get_type)();
};

struct context {
    /* context class */
    const struct class* self;
    /* enter context */
    void (*enter)(const struct class* self);
    /* leaves context */
    const struct class* (*leave)();
    /* reads data */
    const void* (*get_data)();
    /* writes data */
    void (*set_data)(const void* data);
};

/* initializes the class instance */
void class_init(struct class** const current);
/* destroys the class instance */
void class_destroy(struct class** const current);

#endif // _LIST_DATA_H_
