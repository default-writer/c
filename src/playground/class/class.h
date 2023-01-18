#ifndef _PLAYGROUND_CLASS_H_
#define _PLAYGROUND_CLASS_H_

#include "std/common.h"

struct class_data;

struct class {
    /* initialize context */
    void (*init)(struct class_data** current);
    /* destroy context */
    void (*destroy)(struct class_data** current);
    /* enter context */
    void (*push)(struct class_data* self);
    /* leaves context */
    struct class_data* (*pop)();
    /* reads data */
    void* (*get)();
    /* writes data */
    void (*set)(void* data);
};

#endif // _PLAYGROUND_CLASS_H_
