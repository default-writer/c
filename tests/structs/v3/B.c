#include "std/common.h"

#include "B.h"

extern struct_class class_definition;

extern struct_interface_A interface_A_definition;
static const interface_A instance_interface_A = &interface_A_definition;

static const class instance_class = &class_definition;

static object_typeinfo initialize(void);
static void print(object_typeinfo ptr);
static void finalize(object_typeinfo ptr);

typedef struct writable_B {
    A a;
    u64 counter;
} *writable_pointer_B, writable_struct_B;

struct_typeinfo b_typeinfo = {
    .size = sizeof(struct B),
#ifdef USE_MEMORY_DEBUG_INFO
    .name = "B"
#endif
};

static object_typeinfo initialize(void) {
    object_typeinfo ptr = instance_class->create(&b_typeinfo);
    writable_pointer_B b = (writable_pointer_B)ptr->object;
    instance_interface_A->set_counter(&b->a, 1);
    b->counter = 2;
    return ptr;
}

static void print(object_typeinfo ptr) {
    B b = (B)ptr->object;
    printf("counter a: 0x%0llx\n", b->a.counter);
    printf("counter b: 0x%0llx\n", b->counter);
}

static void finalize(object_typeinfo ptr) {
    writable_pointer_B b = (writable_pointer_B)ptr->object;
    instance_interface_A->set_counter(&b->a, 0);
    b->counter = 0;
    instance_class->destroy(ptr);
}

const struct_interface_B interface_B_definition = {
    .initialize = initialize,
    .print = print,
    .finalize = finalize
};
