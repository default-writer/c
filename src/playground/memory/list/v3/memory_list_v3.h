#ifndef _PLAYGROUND_MEMORY_LIST_H_
#define _PLAYGROUND_MEMORY_LIST_H_

#include "memory/api/v3/ref.h"
#include "std/common.h"

struct memory_ref* memory_list_peek(void);
void memory_list_push(struct memory_ref* ptr);
struct memory_ref* memory_list_pop(void);
void memory_list_init(void);
void memory_list_destroy(void);

#endif /* _PLAYGROUND_MEMORY_LIST_H_ */
