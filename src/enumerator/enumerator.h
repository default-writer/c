#ifndef _ENUMERATOR_H_
#define _ENUMERATOR_H_

#include "std/common.h"

struct vm_data_state;

struct enumerator_data {
    struct vm_data_state* current;
};

struct list_data_enumerator {
    void* (*next)(struct enumerator_data* enumerator);
};

struct enumerator_data* list_enumerator_init(void);
void list_enumerator_destroy(struct enumerator_data* enumerator);

#endif // _ENUMERATOR_H_
