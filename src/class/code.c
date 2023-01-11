#include "class/data.h"
#include "common/alloc.h"

struct data {
    /* data pointer */
    void* ptr;
};

LPTR class_get_type(const struct class *class);
void* class_get_data(const struct class *class);
void class_set_data(const struct class *class, void* data);

inline struct class* _new() {
    /* external code allocates memory and resets memory block to zero  */
    struct class *class = _list_alloc(1, size());
    class->data = _list_alloc(1, sizeof(struct data));
    class->get_data = &class_get_data;
    class->set_data = &class_set_data;
    class->get_type = &class_get_type;
    return class;
}

inline void _delete(struct class* ptr) {
    _list_free(ptr->data, sizeof(struct data));
    _list_free(ptr, size());
}

/* gets size of a memory block to allocate */
inline size_t size() {
    /* returns size of a memory block to allocate */
    return sizeof(struct class);
}

/* initializes the new context's head element */
/* as a result, new memory block will be allocated */
/* current context pointer set to zero */
void class_init(struct class** const current) {
    /* gets the current memory pointer */
    const struct class* tmp = *current;
    /* sets the current memory pointer */
    if (tmp == 0) {
        /* creates emtpy data chunk */
        *current = _new();
    }
}

/* destroys the memory stack */
/* frees all memory elements */
/* as a result, memory will be freed */
void class_destroy(struct class** const current) {
    /* gets the current memory pointer */
    struct class* tmp = *current;
    /* checks if pointer is not null */
    if (tmp != 0) {
        /* frees up the memory */
        _delete(tmp);
        /* resets current pointer to 0 */
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