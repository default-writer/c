#include "playground/class/v2/class_v2.h"
#include "common/alloc.h"
#include "playground/list/v2/list_v2.h"

/* list definition */
extern const struct list list_v2;
/* class data definition */
// extern struct class_data class_data;

/* class data */
struct class_data {
    /* list pointer */
    struct list_data* list;
    /* data pointer */
    void* ptr;
    /* list */
};

/* private */

/* list definition */
static const struct list* list = &list_v2;

/* creates the class instance */
static struct class_data* _new(void);
/* deletes the class instance */
static void _delete(struct class_data* ptr);

/* proxy for the class function get_data() */
static void* class_get(struct class_data* class);
/* proxy for the class function set_data( void*)*/
static void class_set(struct class_data* class, void* data);

/* size of a memory block to allocate */
static const size_t _size = sizeof(struct class_data);

/* allocates memory pointer */
static struct class_data* _new(void) {
    /* returns class object */
    return global_alloc(_size);
}

/* releases memory pointer */
static void _delete(struct class_data* class) {
    /* releases the pointer */
    global_free(class, _size);
}

/* initializes the new context's head element */
static struct class_data* class_new(struct list_data* ptr) {
    /* creates emtpy data chunk */
    struct class_data* _class = _new();
    _class->list = ptr;
    return _class;
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
    list->push(class->list, class);
}

static struct class_data* class_pop(struct class_data* class) {
    /* pops from the list */
    return list->pop(class->list);
}

static void* class_get(struct class_data* class) {
    /* returns data */
    return class_get_data(list->peek(class->list));
}

static void class_set(struct class_data* class, void* data) {
    /* updates the data */
    class_set_data(list->peek(class->list), data);
}

/* public */

const struct class class_definition_v2 = {
    .new = class_new,
    .delete = class_delete,
    /* generic methods */
    .push = class_push,
    .pop = class_pop,
    .get = class_get,
    .set = class_set
};
