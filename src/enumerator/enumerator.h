#ifndef _LIST_ENUMERATOR_H_
#define _LIST_ENUMERATOR_H_

#include "std/common.h"

struct enumerator_data {
    void** value;
    void** current;
    void** initial;
};

struct list_data_enumerator {
    void (*init)(struct enumerator_data** enumerator);
    void (*destroy)(struct enumerator_data** enumerator);
    void* (*next)(struct enumerator_data** enumerator);
    void (*reset)(struct enumerator_data** enumerator);
};

#endif // _LIST_ENUMERATOR_H_
