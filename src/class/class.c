#include "class/class.h"
#include "std/headers.h"

static object_typeinfo class_create(typeinfo t);
static void class_destroy(object_typeinfo b);

const _class class_methods = {
    .create = class_create,
    .destroy = class_destroy
};

static object_typeinfo class_create(typeinfo t) {
    _object_typeinfo ti = {
        .ptr = calloc(1, t->size),
        .typeinfo = t
    };
    object_typeinfo bp = calloc(1, sizeof(_object_typeinfo));
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
    free(b->ptr);
    free(b);
}

const _class class_definition = {
    .create = class_create,
    .destroy = class_destroy
};