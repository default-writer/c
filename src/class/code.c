#include "class/data.h"
#include "common/alloc.h"
#include "list-micro/data.h"

/* class definition */
extern struct class class_definition;
/* list definition */
extern struct list list_micro_definition;
/* context definition */
extern struct context context_definition;

/* class data */
struct data {
    /* data pointer */
    void* ptr;
};

/* creates the class instance */
static struct class* _new();
/* deletes the class instance */
static void _delete(struct class* ptr);
/* returns class instance size */
static size_t _size();

/* returns class type id */
__u_int64_t class_get_type();
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

/* allocates memory pointer */
static struct class* _new() {
    // pointer to list functions definitions
    struct list* list = &list_micro_definition;
    // pointer to context functions definitions
    struct context* context = &context_definition;
    // class definition
    struct class* definition = &class_definition;
    // pointer to list data structure
    struct list_data* ctx = 0;
    // init list
    list->init(&ctx);
    // set current context
    context->list = ctx;
    /* allocates memory */
    struct class* class = _list_alloc(1, _size());
    /* copy class defintion to the new structure */
    memcpy(class, definition, _size());
    /* allocates nested memory pointer */
    class->data = _list_alloc(1, sizeof(struct data));
    // returns class object
    return class;
}

/* releases memory pointer */
static void _delete(struct class* class) {
    // pointer to list functions definitions
    struct list* list = &list_micro_definition;
    // pointer to context functions definitions
    struct context* context = &context_definition;
    // pointer to list data structure
    struct list_data* ctx = context->list;
    // reset current context
    context->list = 0;
    // destroys list
    list->destroy(&ctx);
    /* releases nested memory pointer */
    _list_free(class->data, sizeof(struct data));
    /* releases the pointer */
    _list_free(class, _size());
}

/* initializes the new context's head element */
void class_init(struct class** current) {
    /* gets the current memory pointer */
    struct class* tmp = *current;
    /* sets the current memory pointer */
    if (tmp == 0) {
        /* creates emtpy data chunk */
        *current = _new();
    }
}

/* destroys the memory stack */
void class_destroy(struct class** current) {
    /* gets the current memory pointer */
    struct class* tmp = *current;
    /* checks if pointer is not null */
    if (tmp != 0) {
        /* releases the pointer */
        _delete(tmp);
        /* resets current pointer to 0 */
        *current = 0;
    }
}

__u_int64_t class_get_type() {
    struct class* type = &class_definition;
    return (__u_int64_t)type;
}

void* class_get_data(struct class* class) {
    return class->data->ptr;
}

void class_set_data(struct class* class, void* data) {
    class->data->ptr = data;
}

void context_enter(struct class* class) {
    // pointer to list functions definitions
    struct list* list = &list_micro_definition;
    // pointer to context functions definitions
    struct context* context = &context_definition;
    // pushes to the list
    list->push(&context->list, class);
}

struct class* context_leave() {
    // pointer to list functions definitions
    struct list* list = &list_micro_definition;
    // pointer to context functions definitions
    struct context* context = &context_definition;
    // pops from the list 
    return list->pop(&context->list);
}

void* context_get_data() {
    // pointer to list functions definitions
    struct list* list = &list_micro_definition;
    // pointer to context functions definitions
    struct context* context = &context_definition;
    // pointer to class function definitions
    struct class* class = &class_definition;
    // returns data
    return class->get_data(list->peek(&context->list));
}

void context_set_data(void* data) {
    // pointer to list functions definitions
    struct list* list = &list_micro_definition;
    // pointer to context functions definitions
    struct context* context = &context_definition;
    // pointer to class function definitions
    struct class* class = &class_definition;
    // updates the data
    class->set_data(list->peek(&context->list), data);
}

struct class class_definition = {
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