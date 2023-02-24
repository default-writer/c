#ifndef _LIST_ENUMERATOR_H_
#define _LIST_ENUMERATOR_H_

#include "std/common.h"

struct enumerator_data {
    void** value;
    void** current;
};

struct list_data_enumerator {
    struct enumerator_data* (*init)(void);
    void (*destroy)(struct enumerator_data* enumerator);
    void* (*next)(struct enumerator_data* ptr);
};

#endif // _LIST_ENUMERATOR_H_
