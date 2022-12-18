#ifndef _COMMON_OBJECT_H_
#define _COMMON_OBJECT_H_

#include "std/common.h"

void list_init(struct list_data** const current, struct list_data* (*new)());
void list_destroy(struct list_data** const current, void (*delete)(struct list_data*), struct list_data* (*next)(struct list_data*));

#endif // _COMMON_OBJECT_H_