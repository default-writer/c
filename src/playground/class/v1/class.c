#include "playground/class/class.h"
#include "common/alloc.h"
#include "list-micro/data.h"

/* class definition */
extern const struct class class_definition;
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
static struct list_data* class_data_list;
/* list definition */
static const struct list* list = &list_micro_definition;

/* creates the class instance */
static struct class_data* _new();
/* deletes the class instance */
static void _delete(struct class_data* ptr);
/* returns class instance size */
static size_t _size();

/* returns class type id */
static u64 class_get_type();
/* returns class data*/
static void* class_get_data(struct class_data* class);
/* sets the class data */
static void class_set_data(struct class_data* class, void* data);

/* sets the class context */
static void context_enter(struct class_data* class);
/* resets the class context */
static struct class* context_leave();
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
    // class definition
    const struct class* definition = &class_definition;
    // pointer to list data structure
    struct list_data* ctx = 0;
    // initializes the list
    list->init(&ctx);
    // sets the list data pointer
    class_data_list = ctx;
    /* allocates memory */
    struct class_data* ptr = _list_alloc(1, _size());
    /* copy class defintion to the new structure */
    memcpy(ptr, definition, _size());
    // returns class object
    return ptr;
}

/* releases memory pointer */
static void _delete(struct class_data* class) {
    // pointer to list data structure
    struct list_data* ctx = class_data_list;
    /* releases the pointer */
    _list_free(class, _size());
    // resets the list data pointer
    class_data_list = 0;
    // destroys the list
    list->destroy(&ctx);
}

static void* class_get_data(struct class_data* class) {
    return class->ptr;
}

static void class_set_data(struct class_data* class, void* data) {
    class->ptr = data;
}

static void class_push(struct class_data* class) {
    // pushes to the list
    list->push(&class_data_list, class);
}

static struct class_data* class_pop() {
    // pops from the list
    return list->pop(&class_data_list);
}

static void* class_get() {
    // returns data
    return class_get_data(list->peek(&class_data_list));
}

static void class_set(void* data) {
    // updates the data
    class_set_data(list->peek(&class_data_list), data);
}

/* public */

/* initializes the new context's head element */
void class_init(struct class_data** current) {
    /* gets the current memory pointer */
    struct class_data* tmp = *current;
    /* sets the current memory pointer */
    if (tmp == 0) {
        /* creates emtpy data chunk */
        *current = _new();
    }
}

/* destroys the memory stack */
void class_destroy(struct class_data** current) {
    /* gets the current memory pointer */
    struct class_data* tmp = *current;
    /* checks if pointer is not null */
    if (tmp != 0) {
        /* releases the pointer */
        _delete(tmp);
        /* resets current pointer to 0 */
        *current = 0;
    }
}

const struct class class_definition = {
    // generic methods
    .push = class_push,
    .pop = class_pop,
    .get = class_get,
    .set = class_set
};