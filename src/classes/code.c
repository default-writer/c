#include "classes/data.h"
#include "common/alloc.h"

LPTR class_get_type();

struct class_methods* _new() {
    /* external code allocates memory and resets memort block to zero  */
    struct class_methods *class = _list_alloc(1, size());
    class->get_type = &class_get_type;
    return class;
}

void _delete(struct class_methods* ptr) {
    _list_free(ptr, size());
}

/* gets size of a memory block to allocate */
size_t size() {
    /* returns size of a memory block to allocate */
    return sizeof(struct class_methods);
}


/* initializes the new context's head element */
/* as a result, new memory block will be allocated */
/* current context pointer set to zero */
void class_init(struct class_methods** const current) {
    /* sets current context's head element */
    *current = _new();
}

/* destroys the memory stack */
/* frees all memory elements */
/* as a result, memory will be freed */
void class_destroy(struct class_methods** const current) {
    /* get current context's head */
    /* assigns currently selected item pointer to temporary */
    struct class_methods* tmp = *current;
    /* if not already freed */
    if (tmp != 0) {
        /* gets temporary pointer value */
        struct class_methods* ptr = tmp;
        /* gets prev pointer value */
        _delete(ptr);
        /* all stack items are processed */
        *current = 0;
    }
}

const struct class_methods class_definition;

LPTR class_get_type()
{
    const struct class_methods* type = &class_definition;
    return (LPTR)type;
}

const struct class_methods class_definition = {
    // generic methods
    .init = class_init,
    .destroy = class_destroy,
    .get_type = class_get_type
};