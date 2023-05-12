#ifndef _PLAYGROUND_CLASS_V2_H_
#define _PLAYGROUND_CLASS_V2_H_

#include "std/common.h"

struct class_data;

struct class {
    /* initialize context */
    struct class_data* (*new)(void);
    /* destroy context */
    void (*delete)(struct class_data* class);
    /* enter context */
    void (*push)(struct class_data* class);
    /* leaves context */
    struct class_data* (*pop)(void);
    /* reads data */
    void* (*get)(void);
    /* writes data */
    void (*set)(void* data);
};

#endif /* _PLAYGROUND_CLASS_V2_H_ */
