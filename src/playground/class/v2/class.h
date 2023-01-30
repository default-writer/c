#ifndef _PLAYGROUND_CLASS_H_
#define _PLAYGROUND_CLASS_H_

#include "std/common.h"

struct class_data;

struct class {
    /* initialize context */
    struct class_data* (*new)();
    /* destroy context */
    void (*delete)(struct class_data* class);
    /* enter context */
    void (*push)(struct list_data* ptr, struct class_data* class);
    /* leaves context */
    struct class_data* (*pop)(struct list_data* ptr);
    /* reads data */
    void* (*get)(struct list_data* ptr);
    /* writes data */
    void (*set)(struct list_data* ptr, void* data);
};

#endif // _PLAYGROUND_CLASS_H_
