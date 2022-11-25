#ifndef _OBJECT_API_H_
#define _OBJECT_API_H_

#include "globals.h"
#include "alloc.h"
#include "data.h"

/* object: vtable definition */
struct object_class {
    /* initialize context */
    void (*init)(struct list** const current);
    /* destroy context */
    void (*destroy)(struct list** const current);
    /* link to self-contained methods structure */
    const struct object_class* self;
};

#endif // _OBJECT_API_H_