#ifndef _B_H_
#define _B_H_

#include "class/class.h"
#include "common/alloc.h"
#include "std/common.h"

#include "A.h"

typedef struct B {
    const A a;
    const u64 counter;
} const *B, struct_B;

typedef struct interface_B struct_interface_B;
typedef struct_interface_B* interface_B;

struct interface_B {
    object_typeinfo (*initialize)(void);
    void (*print)(object_typeinfo ptr);
    void (*finalize)(object_typeinfo ptr);
};

#endif /* _B_H_ */