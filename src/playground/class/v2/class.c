#include "playground/class/class.h"
#include "common/alloc.h"
#include "playground/list/list.h"

/* list definition */
extern const struct list list_v1;
/* class data definition */
extern struct class_data class_data;

/* class data */
struct class_data {
    /* data pointer */
    void* ptr;
    /* list */
};

/* private */

/* list definition */
static const struct list* list = &list_v1;

/* creates the class instance */
static struct class_data* _new();
/* deletes the class instance */
static void _delete(struct class_data* ptr);
/* returns class instance size */
static size_t _size();

/* proxy for the class function get_data() */
static void* class_get();
/* proxy for the class function set_data( void*)*/
static void class_set(void* data);

/* gets size of a memory block to allocate */
static size_t _size() {
    /* returns size of a memory block to allocate */
    return sizeof(struct class_data);
}

/* allocates memory pointer */
static struct class_data* _new() {
    // returns class object
    return _list_alloc(1, _size());
}

/* releases memory pointer */
static void _delete(struct class_data* class) {
    /* releases the pointer */
    _list_free(class, _size());
}

/* initializes the new context's head element */
static struct class_data* class_new() {
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
    // pushes to the list
    list->push(class);
}

static struct class_data* class_pop() {
    // pops from the list
    return list->pop();
}

static void* class_get() {
    // returns data
    return class_get_data(list->peek());
}

static void class_set(void* data) {
    // updates the data
    class_set_data(list->peek(), data);
}

/* public */

const struct class class_definition_v2 = {
    .new = class_new,
    .delete = class_delete,
    // generic methods
    .push = class_push,
    .pop = class_pop,
    .get = class_get,
    .set = class_set
};