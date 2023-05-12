#include "playground/class/v1/class_v1.h"
#include "common/alloc.h"
#include "list-micro/data.h"

/* list definition */
extern const struct list list_micro_definition;
/* class data definition */
extern struct class_data class_data;

/* class data */
struct class_data {
    /* data pointer */
    void* ptr;
    /* list */
};

/* private */

/* class data list */
static struct list_data list_data;
/* class data list reference */
static struct list_data* class_list_data = &list_data;

/* list definition */
static const struct list* list = &list_micro_definition;

/* creates the class instance */
static struct class_data* _new(void);
/* deletes the class instance */
static void _delete(struct class_data* ptr);
/* returns class instance size */
static size_t _size(void);

/* proxy for the class function get_data() */
static void* class_get(void);
/* proxy for the class function set_data( void*)*/
static void class_set(void* data);

/* gets size of a memory block to allocate */
static size_t _size(void) {
    /* returns size of a memory block to allocate */
    return sizeof(struct class_data);
}

/* allocates memory pointer */
static struct class_data* _new(void) {
    /* returns class object */
    return _list_alloc(_size());
}

/* releases memory pointer */
static void _delete(struct class_data* class) {
    /* releases the pointer */
    _list_free(class, _size());
}

/* initializes the new context's head element */
static struct class_data* class_new(void) {
    /* creates emtpy data chunk */
    return _new();
}

/* destroys the memory stack */
static void class_delete(struct class_data* class) {
    /* releases the pointer */
    _delete(class);
}

static void* class_get_data(struct class_data* class) {
    return class->ptr;
}

static void class_set_data(struct class_data* class, void* data) {
    class->ptr = data;
}

static void class_push(struct class_data* class) {
    /* pushes to the list */
    list->push(&class_list_data, class);
}

static struct class_data* class_pop(void) {
    /* pops from the list */
    return list->pop(&class_list_data);
}

static void* class_get(void) {
    /* returns data */
    return class_get_data(list->peek(&class_list_data));
}

static void class_set(void* data) {
    /* updates the data */
    class_set_data(list->peek(&class_list_data), data);
}

/* public */

const struct class class_definition_v1 = {
    .new = class_new,
    .delete = class_delete,
    /* generic methods */
    .push = class_push,
    .pop = class_pop,
    .get = class_get,
    .set = class_set
};
