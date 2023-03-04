#ifndef _MEMORY_REF_H_
#define _MEMORY_REF_H_

#include "std/common.h"

struct memory_ref {
    void* next;
    void* prev;
    u64 size;
};

#endif // _MEMORY_REF_H_
