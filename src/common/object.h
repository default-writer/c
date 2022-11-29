#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <stdlib.h>

struct list_data;
struct list_data* next(struct list_data *ptr);
struct list_data* new();

void list_init(struct list_data** const current, struct list_data* (*new)());
void list_destroy(struct list_data** const current, struct list_data* (*next)(struct list_data*));

#endif // _OBJECT_H_