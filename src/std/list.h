#ifndef _STD_LIST_H_
#define _STD_LIST_H_

#include "std/common.h"

struct list_data* list_next(struct list_data *ptr);
void* list_data(struct list_data *ptr);

#endif // _STD_LIST_H_
