#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <stdlib.h>

struct list_data;

struct list_data* new();
struct list_data* next(struct list_data *ptr);
void* data(struct list_data *ptr);
void delete(struct list_data* ptr);

void list_init(struct list_data** const current, struct list_data* (*new)());
void list_destroy(struct list_data** const current, void (*delete)(struct list_data*), struct list_data* (*next)(struct list_data*));

#endif // _OBJECT_H_