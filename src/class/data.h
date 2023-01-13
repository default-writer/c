#ifndef _LIST_DATA_H_
#define _LIST_DATA_H_

#include "std/common.h"

struct data;

struct class {
    /* private data definition */
    struct data* data;
    /* reads data */
    void* (*get_data)(struct class* class);
    /* writes data */
    void (*set_data)(struct class* class, void* data);
    /* returns current class type id */
    __u_int64_t (*get_type)();
};

struct context {
    /* list */
    struct list_data* list;
    /* enter context */
    void (*enter)(struct class* self);
    /* leaves context */
    struct class* (*leave)();
    /* reads data */
    void* (*get_data)();
    /* writes data */
    void (*set_data)(void* data);
};

/* initializes the class instance */
void class_init(struct class** current);
/* destroys the class instance */
void class_destroy(struct class** current);

#endif // _LIST_DATA_H_
