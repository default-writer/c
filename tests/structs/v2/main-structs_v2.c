#include "std/common.h"
#include <string.h>

typedef struct object _object;
typedef struct typeinfo _typeinfo;
typedef struct object_typeinfo _object_typeinfo;

typedef _object* object;
typedef _typeinfo* typeinfo;
typedef _object_typeinfo* object_typeinfo;

typedef const struct class* class;

object_typeinfo class_create(typeinfo t);
void class_destroy(object_typeinfo b);

struct object { };

struct class {
    object_typeinfo (*create)(typeinfo t);
    void (*destroy)(object_typeinfo base);
};

struct typeinfo {
    const size_t size;
    const char* name;
};

struct object_typeinfo {
    const object ptr;
    const typeinfo typeinfo;
};

const struct class class_methods = {
    .create = class_create,
    .destroy = class_destroy
};

object_typeinfo class_create(typeinfo t) {
    _object_typeinfo ti = {
        .ptr = calloc(1, t->size),
        .typeinfo = t
    };
    object_typeinfo bp = calloc(1, sizeof(_object_typeinfo));
    memcpy(bp, &ti, sizeof(_object_typeinfo));
    printf("creating type %s of size %ld (+ size %ld)\n", t->name, t->size, sizeof(_object_typeinfo));
    return bp;
}

void class_destroy(object_typeinfo b) {
    printf("deleting type %s of size %ld (+ size %ld)\n", b->typeinfo->name, b->typeinfo->size, sizeof(_object_typeinfo));
    free(b->ptr);
    free(b);
}

int main(void) {
    typedef struct B _B;
    typedef struct B* B;

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

    class instance = &class_methods;

    object_typeinfo class_object = instance->create(&b_typeinfo);

    B b = (B)class_object->ptr;

    b->base.counter_a = 1;
    b->counter_b = 2;

    printf("counter a: 0x%0llx\n", b->base.counter_a);
    printf("counter b: 0x%0llx\n", b->counter_b);

    instance->destroy(class_object);

    return 0;
}
