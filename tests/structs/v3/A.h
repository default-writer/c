#ifndef _A_H_
#define _A_H_

#include "class/class.h"
#include "common/alloc.h"
#include "std/common.h"

typedef struct A {
    const u64 counter;
} A;

typedef const A* readonly_A;

typedef struct interface_A struct_interface_A;
typedef struct_interface_A* interface_A;

struct interface_A {
    void (*set_counter)(A* a, u64 value);
};

#endif /* _A_H_ */