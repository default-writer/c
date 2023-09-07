#include "type.h"
#include "../new.h"

typedef struct object_data {
    int version;
} *object_data_pointer, object_data;

typedef struct object_object {
    object_data_pointer data;
} object_object;

object_type_pointer object;

static object_pointer object_new() {
    object_pointer ptr = calloc(1, sizeof(object_object) + sizeof(object_data));
    ptr->data = (object_data_pointer)(ptr + 1);
    return ptr;
}

static object_pointer object_create() {
    object_pointer ptr = object_new();
    ptr->data->version = 1;
    printf("object create v%d\n", ptr->data->version);
    return ptr;
}

static void object_destroy(object_pointer ptr) {
    printf("object destroy v%d\n", ptr->data->version);
    free(ptr);
}

static void __attribute__((constructor)) object_type_init() {
    object = calloc(1, sizeof(object_type));
    object->create = object_create;
    object->destroy = object_destroy;
}

static void __attribute__((destructor)) object_type_free() {
    free(object);
}