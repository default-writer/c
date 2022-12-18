#ifndef _COMMON_PRINT_H_
#define _COMMON_PRINT_H_

#include "std/common.h"
#include "std/list.h"

void list_print_head(struct list_data** const current, void* (*list_data)(struct list_data*));
void list_print(struct list_data** const current, struct list_data* (*list_next)(struct list_data*), void* (*list_data)(struct list_data*));

#endif // _COMMON_PRINT_H_
