#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <stdlib.h>

#include "std/api.h"

size_t size();
struct list_data* _new();
struct list_data* _next(struct list_data *ptr);
void* _data(struct list_data *ptr);
void _delete(struct list_data* ptr);

void list_init(struct list_data** const current, struct list_data* (*new)());
void list_destroy(struct list_data** const current, void (*delete)(struct list_data*), struct list_data* (*next)(struct list_data*));

#endif // _OBJECT_H_