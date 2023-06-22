#include "std/common.h"

#include "A.h"

extern struct_class class_definition;

static const class instance_class = &class_definition;

typedef struct writable_A {
    u64 counter;
} *writable_pointer_A, writable_struct_A;

static void set_counter(A* a, u64 value);

static void set_counter(A* a, u64 value) {
    writable_pointer_A ptr = (writable_pointer_A)a;
    ptr->counter = value;
}

struct_typeinfo a_typeinfo = {
    .size = sizeof(struct A),
#ifdef USE_MEMORY_DEBUG_INFO
    .name = "A"
#endif
};

const struct_interface_A interface_A_definition = {
    .set_counter = set_counter
};
