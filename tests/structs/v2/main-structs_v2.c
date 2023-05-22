#include "class/class.h"
#include "common/alloc.h"

extern const _class class_definition;

const class instance = &class_definition;

int main(void) {
    typedef struct B _B;
    typedef _B* B;

    struct A {
        u64 counter_a;
    };

    struct B {
        struct A base;
        u64 counter_b;
    };

    _typeinfo b_typeinfo = {
        .size = sizeof(_B),
        .name = "B"
    };

    object_typeinfo class_object = instance->create(&b_typeinfo);

    B b = (B)class_object->ptr;

    b->base.counter_a = 1;
    b->counter_b = 2;

    printf("counter a: 0x%0llx\n", b->base.counter_a);
    printf("counter b: 0x%0llx\n", b->counter_b);

    instance->destroy(class_object);

#ifdef USE_MEMORY_DEBUG_INFO
    global_statistics();
#endif
    return 0;
}
