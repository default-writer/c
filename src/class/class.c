#include "class/class.h"
#include "common/alloc.h"

static object_typeinfo class_create(typeinfo t);
static void class_destroy(object_typeinfo b);

const _class class_methods = {
    .create = class_create,
    .destroy = class_destroy
};

static object_typeinfo class_create(typeinfo t) {
    object_typeinfo bp = global_alloc(sizeof(_object_typeinfo));
    _object_typeinfo ti = {
        .ptr = global_alloc(t->size),
        .typeinfo = t
    };
    memcpy(bp, &ti, sizeof(_object_typeinfo));
#ifdef USE_MEMORY_DEBUG_INFO
    printf("creating type %s of size %ld (+ %ld)\n", t->name, t->size, sizeof(_object_typeinfo));
#endif
    return bp;
}

static void class_destroy(object_typeinfo b) {
#ifdef USE_MEMORY_DEBUG_INFO
    printf("deleting type %s of size %ld (+ %ld)\n", b->typeinfo->name, b->typeinfo->size, sizeof(_object_typeinfo));
#endif
    global_free(b->ptr, b->typeinfo->size);
    global_free(b, sizeof(_object_typeinfo));
}

const _class class_definition = {
    .create = class_create,
    .destroy = class_destroy
};