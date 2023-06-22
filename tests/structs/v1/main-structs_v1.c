#include "common/alloc.h"
#include "std/common.h"

typedef struct object* object;
typedef const struct typeinfo* typeinfo;
typedef const struct object_typeinfo* object_typeinfo;

object base_create(const typeinfo ti);
void base_destroy(const object_typeinfo ptr);

struct object { };

struct base {
    object (*create)(const typeinfo ti);
    void (*destroy)(const object_typeinfo ptr);
};

struct typeinfo {
    const size_t size;
#ifdef USE_MEMORY_DEBUG_INFO
    const char* name;
#endif
};

struct object_typeinfo {
    const object object;
    const typeinfo typeinfo;
};

const struct base base_methods = {
    .create = base_create,
    .destroy = base_destroy
};

object base_create(const typeinfo t) {
    object obj = (object)global_alloc(t->size);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("creating type %s of size %ld\n", t->name, t->size);
#endif
    return obj;
}

void base_destroy(const object_typeinfo ptr) {
#ifdef USE_MEMORY_DEBUG_INFO
    printf("deleting type %s of size %ld\n", ptr->typeinfo->name, ptr->typeinfo->size);
#endif
    global_free(ptr->object, ptr->typeinfo->size);
}

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

    struct typeinfo ti = {
        .size = sizeof(_B),
#ifdef USE_MEMORY_DEBUG_INFO
        .name = "B"
#endif
    };

    const struct base* base = &base_methods;
    const B b = (B)base->create(&ti);

    b->base.counter_a = 1;
    b->counter_b = 2;

    printf("counter a: 0x%0llx\n", b->base.counter_a);
    printf("counter b: 0x%0llx\n", b->counter_b);

    const struct object_typeinfo obj = {
        .object = (object)b,
#ifdef USE_MEMORY_DEBUG_INFO
        .typeinfo = &ti
#endif
    };

    base->destroy(&obj);

    global_statistics();
    return 0;
}
