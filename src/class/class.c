#include "class/class.h"
#include "common/alloc.h"

static object_typeinfo class_create(typeinfo ti);
static void class_destroy(object_typeinfo ptr);

typedef struct writeable_object_typeinfo {
    object object;
    typeinfo typeinfo;
}* writeable_object_typeinfo;

static object_typeinfo class_create(typeinfo ti) {
    object_typeinfo ptr;
#ifndef USE_MEMCPY
    writeable_object_typeinfo type = global_alloc(sizeof(struct_object_typeinfo));
    type->object = global_alloc(ti->size);
    type->typeinfo = ti;
    ptr = (object_typeinfo)type;
#else
    ptr = global_alloc(sizeof(object_typeinfo));
    struct_object_typeinfo object_ti = {
        .object = global_alloc(t->size),
        .typeinfo = ti
    };
    memcpy(ptr, &object_ti, sizeof(object_typeinfo));
#endif
#ifdef USE_MEMORY_DEBUG_INFO
    printf("creating type %s of size %ld (+ %ld)\n", ti->name, ti->size, sizeof(struct_object_typeinfo));
#endif
    return ptr;
}

static void class_destroy(object_typeinfo ptr) {
#ifdef USE_MEMORY_DEBUG_INFO
    printf("deleting type %s of size %ld (+ %ld)\n", ptr->typeinfo->name, ptr->typeinfo->size, sizeof(struct_object_typeinfo));
#endif
    global_free(ptr->object, ptr->typeinfo->size);
    global_free(ptr, sizeof(struct_object_typeinfo));
}

const struct_class class_definition = {
    .create = class_create,
    .destroy = class_destroy
};