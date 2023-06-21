#include "class/class.h"
#include "common/alloc.h"

static object_typeinfo class_create(typeinfo t);
static void class_destroy(object_typeinfo b);

const struct_class class_methods = {
    .create = class_create,
    .destroy = class_destroy
};

static object_typeinfo class_create(typeinfo t) {
    object_typeinfo bp;
#ifndef USE_MEMCPY
    typedef union {
        struct_object_typeinfo typeinfo;
        struct {
            object ptr;
            typeinfo typeinfo;
        } set;
    }* writeable_object_typeinfo;
    writeable_object_typeinfo type = global_alloc(sizeof(struct_object_typeinfo));
    type->set.ptr = global_alloc(t->size);
    type->set.typeinfo = t;
    bp = &type->typeinfo;
#else
    bp = global_alloc(sizeof(_object_typeinfo));
    struct_object_typeinfo ti = {
        .ptr = global_alloc(t->size),
        .typeinfo = t
    };
    memcpy(bp, &ti, sizeof(_object_typeinfo));
#endif
#ifdef USE_MEMORY_DEBUG_INFO
    printf("creating type %s of size %ld (+ %ld)\n", t->name, t->size, sizeof(struct_object_typeinfo));
#endif
    return bp;
}

static void class_destroy(object_typeinfo b) {
#ifdef USE_MEMORY_DEBUG_INFO
    printf("deleting type %s of size %ld (+ %ld)\n", b->typeinfo->name, b->typeinfo->size, sizeof(struct_object_typeinfo));
#endif
    global_free(b->ptr, b->typeinfo->size);
    global_free(b, sizeof(struct_object_typeinfo));
}

const struct_class class_definition = {
    .create = class_create,
    .destroy = class_destroy
};