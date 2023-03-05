#ifndef _PLAYGROUND_MEMORY_LIST_H_
#define _PLAYGROUND_MEMORY_LIST_H_

#include "std/common.h"

void* list_peek(void);
void list_push(void* data);
void* list_pop(void);
void list_init(void);
void list_destroy(void);

#endif // _PLAYGROUND_MEMORY_LIST_H_
