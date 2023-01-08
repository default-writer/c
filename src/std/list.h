#ifndef _STD_LIST_H_
#define _STD_LIST_H_

#include "std/common.h"

struct list_data* _new();
void _delete(struct list_data* ptr);
size_t size();
struct list_data* list_next(struct list_data *ptr);
const void* list_data(const struct list_data *ptr);
// void list_print_head(struct list_data** const current, const void* (*list_data)(const struct list_data*));
// void list_print(struct list_data** const current, struct list_data* (*list_next)(struct list_data*), const void* (*list_data)(const struct list_data*));

#endif // _STD_LIST_H_
