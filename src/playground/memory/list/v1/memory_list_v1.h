#ifndef _PLAYGROUND_MEMORY_LIST_H_
#define _PLAYGROUND_MEMORY_LIST_H_

#include "std/common.h"

void* memory_list_peek(void);
void memory_list_push(void* data);
void* memory_list_pop(void);
void memory_list_init(void);
void memory_list_destroy(void);

#endif // _PLAYGROUND_MEMORY_LIST_H_