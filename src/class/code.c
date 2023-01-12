#include "class/data.h"
#include "common/alloc.h"

/* class data */
struct data {
    /* data pointer */
    const void* ptr;
};

/* creates the class instance */
inline struct class* _new();
/* deletes the class instance */
inline void _delete(struct class* ptr);
/* returns class instance size */
inline size_t size();

/* returns class type id */
inline LPTR class_get_type();
/* returns class data*/
inline const void* class_get_data(const struct class *class);
/* sets the class data */
inline void class_set_data(const struct class *class, const void* data);

/* sets the class context */
inline void context_enter(const struct class* class);
/* resets the class context */
inline const struct class* context_leave();
/* proxy for the class function get_data() */
inline const void* context_get_data();
/* proxy for the class function set_data(const void*)*/
inline void context_set_data(const void* data);

inline struct class* _new() {
    /* external code allocates memory and resets memory block to zero  */
    struct class *class = _list_alloc(1, size());
    class->data = _list_alloc(1, sizeof(struct data));
    class->get_data = class_get_data;
    class->set_data = class_set_data;
    class->get_type = class_get_type;
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
inline void class_init(struct class** const current) {
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
inline void class_destroy(struct class** const current) {
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

/* class definition */
const struct class class_definition;
/* context definition */
struct context context_definition;

inline LPTR class_get_type() {
    const struct class* type = &class_definition;
    return (LPTR)type;
}

inline const void* class_get_data(const struct class *class) {
    return class->data->ptr;
}

inline void class_set_data(const struct class *class, const void* data) {
    class->data->ptr = data;
}

inline void context_enter(const struct class* class) {
    struct context *ctx = &context_definition;
    ctx->self = class;
}

inline const struct class* context_leave() {
    struct context *ctx = &context_definition;
    const struct class* class = ctx->self;
#ifdef USE_MEMORY_CLEANUP
    ctx->self = 0;
#endif
    return class;
}

inline const void* context_get_data() {
    const struct context* context = &context_definition;
    const struct class* class = &class_definition;
    return class->get_data(context->self);
}

inline void context_set_data(const void* data) {
    const struct context* context = &context_definition;
    const struct class* class = &class_definition;
    class->set_data(context->self, data);
}

const struct class class_definition = {
    // generic methods
    .get_type = class_get_type,
    .get_data = class_get_data,
    .set_data = class_set_data
};

struct context context_definition = {
    .enter = context_enter,
    .leave = context_leave,
    .get_data = context_get_data,
    .set_data = context_set_data
};