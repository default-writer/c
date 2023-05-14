#ifndef _MEMORY_API_COMMON_REF_H_
#define _MEMORY_API_COMMON_REF_H_

#include "std/common.h"

struct memory_ref {
    struct memory_ref* prev;
    struct memory_ref* next;
    struct memory_ref* cache;
    u64 size;
};

#endif /* _MEMORY_API_COMMON_REF_H_ */
