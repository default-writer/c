#ifndef _PLAYGROUND_CLASS_V1_H_
#define _PLAYGROUND_CLASS_V1_H_

#include "playground/list/v2/list_v2.h"

struct class_data;

struct class {
    /* initialize context */
    struct class_data* (*new)(struct list_data* ptr);
    /* destroy context */
    void (*delete)(struct class_data* class);
    /* enter context */
    void (*push)(struct class_data* class);
    /* leaves context */
    struct class_data* (*pop)(struct class_data* class);
    /* reads data */
    void* (*get)(struct class_data* class);
    /* writes data */
    void (*set)(struct class_data* class, void* data);
};

#endif /* _PLAYGROUND_CLASS_V1_H_ */
