#ifndef _POINTER_TYPES_LIST_H_
#define _POINTER_TYPES_LIST_H_

#include "std/common.h"

struct list_methods {
    u64 (*alloc)(void);
    u64 (*free)(u64);
    u64 (*peek)(u64 list_ptr);
    u64 (*pop)(u64 list_ptr);
    void (*push)(u64 list_ptr, u64 ptr);
    u64 (*size)(u64 ptr);
};

#endif /* _POINTER_TYPES_LIST_H_ */