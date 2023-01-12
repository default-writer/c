#include "class/data.h"
#include "common/alloc.h"
#include "list-micro/data.h"

extern struct list list_micro_definition;

/* class data */
struct data {
    /* data pointer */
    void* ptr;
    /* list */
    struct list_data* ctx;
};

/* creates the class instance */
static struct class* _new();
/* deletes the class instance */
static void _delete(struct class* ptr);
/* returns class instance size */
static size_t _size();

/* returns class type id */
LPTR class_get_type();
/* returns class data*/
void* class_get_data(struct class* class);
/* sets the class data */
void class_set_data(struct class* class, void* data);

/* sets the class context */
void context_enter(struct class* class);
/* resets the class context */
struct class* context_leave();
/* proxy for the class function get_data() */
void* context_get_data();
/* proxy for the class function set_data( void*)*/
void context_set_data(void* data);

/* gets size of a memory block to allocate */
static size_t _size() {
    /* returns size of a memory block to allocate */
    return sizeof(struct class);
}

static struct class* _new() {
    const struct list* list = &list_micro_definition;
    struct list_data* ctx = 0;
    // init list
    list->init(&ctx);
    /* external code allocates memory and resets memory block to zero  */
    struct class* class = _list_alloc(1, _size());
    class->data = _list_alloc(1, sizeof(struct data));
    class->get_data = class_get_data;
    class->set_data = class_set_data;
    class->get_type = class_get_type;
    class->data->ctx = ctx;
    return class;
}

static void _delete(struct class* class) {
    const struct list* list = &list_micro_definition;
    struct list_data* ctx = class->data->ctx;
    // destroys list
    list->destroy(&ctx);
    _list_free(class->data, sizeof(struct data));
    _list_free(class, _size());
}

/* initializes the new context's head element */
/* as a result, new memory block will be allocated */
/* current context pointer set to zero */
void class_init(struct class** current) {
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
void class_destroy(struct class** current) {
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

LPTR class_get_type() {
    const struct class* type = &class_definition;
    return (LPTR)type;
}

void* class_get_data(struct class* class) {
    return class->data->ptr;
}

void class_set_data(struct class* class, void* data) {
    class->data->ptr = data;
}

void context_enter(struct class* class) {
    struct context* ctx = &context_definition;
    ctx->self = class;
}

struct class* context_leave() {
    struct context* ctx = &context_definition;
    struct class* class = ctx->self;
#ifdef USE_MEMORY_CLEANUP
    ctx->self = 0;
#endif
    return class;
}

void* context_get_data() {
    struct context* context = &context_definition;
    const struct class* class = &class_definition;
    return class->get_data(context->self);
}

void context_set_data(void* data) {
    struct context* context = &context_definition;
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