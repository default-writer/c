#ifndef _PLAYGROUND_CLASS_H_
#define _PLAYGROUND_CLASS_H_

#include "std/common.h"

struct class_data;

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

#endif // _PLAYGROUND_CLASS_H_
