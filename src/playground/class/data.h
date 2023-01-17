#ifndef _LIST_DATA_H_
#define _LIST_DATA_H_

#include "std/common.h"

struct data;

struct class_data {
    /* private data definition */
    struct data* data;
};

struct class {
    /* reads data */
    void* (*get_data)(struct class_data* class);
    /* writes data */
    void (*set_data)(struct class_data* class, void* data);
    /* returns current class type id */
    u64 (*get_type)();
    /* enter context */
    void (*push)(struct class_data* self);
    /* leaves context */
    struct class_data* (*pop)();
    /* reads data */
    void* (*get)();
    /* writes data */
    void (*set)(void* data);
};

/* initializes the class instance */
void class_init(struct class_data** current);
/* destroys the class instance */
void class_destroy(struct class_data** current);

#endif // _LIST_DATA_H_
