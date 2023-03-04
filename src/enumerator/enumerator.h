#ifndef _ENUMERATOR_H_
#define _ENUMERATOR_H_

#include "std/common.h"

struct enumerator_data {
    void* current;
};

struct list_data_enumerator {
    void* (*next)(struct enumerator_data* enumerator);
};

struct enumerator_data* list_enumerator_init(void);
void list_enumerator_destroy(struct enumerator_data* enumerator);

#endif // _ENUMERATOR_H_
