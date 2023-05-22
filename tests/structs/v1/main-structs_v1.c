#include "std/common.h"

typedef struct object* object;
typedef const struct typeinfo* typeinfo;
typedef const struct object_typeinfo* object_typeinfo;

object base_create(const typeinfo t);
void base_destroy(const object_typeinfo b);

struct object { };

struct base {
    object (*create)(const typeinfo t);
    void (*destroy)(const object_typeinfo base);
};

struct typeinfo {
    const size_t size;
    const char* name;
};

struct object_typeinfo {
    const object ptr;
    const typeinfo typeinfo;
};

const struct base base_methods = {
    .create = base_create,
    .destroy = base_destroy
};

object base_create(const typeinfo t) {
    printf("creating type %s of size %ld\n", t->name, t->size);
    void* b = calloc(1, t->size);
    object _bz = b;
    return _bz;
}

void base_destroy(const object_typeinfo b) {
    printf("deleting type %s of size %ld\n", b->typeinfo->name, b->typeinfo->size);
    free(b->ptr);
}

int main(void) {

    typedef struct base_B B_struct;
    typedef struct base_B* B;

    struct base_A {
        u64 counter_a;
    };
    struct base_B {
        struct base_A base;
        u64 counter_b;
    };

    struct typeinfo b_info = {
        .size = sizeof(B_struct),
        .name = "B"
    };

    const struct base* base = &base_methods;
    const B b = (B)base->create(&b_info);
    b->base.counter_a = 1;
    b->counter_b = 2;
    printf("counter a: 0x%0llx\n", b->base.counter_a);
    printf("counter b: 0x%0llx\n", b->counter_b);
    const struct object_typeinfo bp = {
        .ptr = (object)b,
        .typeinfo = &b_info
    };
    base->destroy(&bp);
    return 0;
}
