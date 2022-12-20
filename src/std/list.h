#ifndef _STD_LIST_H_
#define _STD_LIST_H_

#include "std/common.h"

struct list_data* list_next(struct list_data *ptr);
const void* list_data(const struct list_data *ptr);

#endif // _STD_LIST_H_
