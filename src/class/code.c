#include "class/data.h"
#include "common/alloc.h"

struct data {
    /* data pointer */
    void* ptr;
};

LPTR class_get_type(const struct class *class);
void* class_get_data(const struct class *class);
void class_set_data(const struct class *class, void* data);

struct class* _new() {
    /* external code allocates memory and resets memort block to zero  */
    struct class *class = _list_alloc(1, size());
    class->data = _list_alloc(1, sizeof(struct data));
    class->get_data = &class_get_data;
    class->set_data = &class_set_data;
    class->get_type = &class_get_type;
    return class;
}

void _delete(struct class* ptr) {
    _list_free(ptr->data, sizeof(struct data));
    _list_free(ptr, size());
}

/* gets size of a memory block to allocate */
size_t size() {
    /* returns size of a memory block to allocate */
    return sizeof(struct class);
}

/* initializes the new context's head element */
/* as a result, new memory block will be allocated */
/* current context pointer set to zero */
void class_init(struct class** const current) {
    /* get current context's head */
    /* assigns currently selected item pointer to temporary */
    const struct class* tmp = *current;
    /* sets current context's head element */
    if (tmp == 0) {
        *current = _new();
    }
}

/* destroys the memory stack */
/* frees all memory elements */
/* as a result, memory will be freed */
void class_destroy(struct class** const current) {
    /* get current context's head */
    /* assigns currently selected item pointer to temporary */
    struct class* tmp = *current;
    /* if not already freed */
    if (tmp != 0) {
        /* gets prev pointer value */
        _delete(tmp);
        /* all stack items are processed */
        *current = 0;
    }
}

const struct class class_definition;

LPTR class_get_type(const struct class *class)
{
    const struct class* type = &class_definition;
    return (LPTR)type;
}

void* class_get_data(const struct class *class)
{
    return class->data->ptr;
}

void class_set_data(const struct class *class, void* data)
{
    class->data->ptr = data;
}

const struct class class_definition = {
    // generic methods
    .get_type = class_get_type,
    .get_data = class_get_data,
    .set_data = class_set_data
};