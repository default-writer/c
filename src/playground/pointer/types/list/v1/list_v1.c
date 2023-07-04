#include "playground/pointer/types/list/v1/list_v1.h"
#include "common/alloc.h"
#include "list-micro/data.h"
#include "playground/pointer/v1/pointer_v1.h"
#include "playground/vm/v1/vm_v1.h"

#define DEFAULT_SIZE 0x100

/* definition */
extern const struct vm vm_definition;
extern const struct list list_micro_definition;
extern const struct pointer_vm_methods vm_methods_definition;
extern void pointer_vm_register_free(function function);

extern struct pointer_data vm_pointer;
static struct pointer_data* base = &vm_pointer;

/* definition */
static const struct vm* vm = &vm_definition;
static const struct list* list = &list_micro_definition;
static const struct pointer_vm_methods* pointer = &vm_methods_definition;

struct list_handler {
    struct list_data* list;
};

/* api */

static u64 list_alloc(void);
static void list_free(u64 ptr);
static void list_push(u64 ptr, u64 data_ptr);
static u64 list_peek(u64 ptr);
static u64 list_pop(u64 ptr);

/* internal */

/* implementation*/

static u64 list_alloc(void) {
    struct pointer* ptr = pointer->alloc(sizeof(struct list_handler), TYPE_LIST);
    struct list_handler* handler = ptr->data;
    list->init(&handler->list);
    u64 data = vm->write(ptr);
#ifdef USE_GC
    list->push(&base->gc, (void*)data);
#endif
    return data;
}

static void list_free(u64 ptr) {
    if (ptr == 0) {
        return;
    }
    struct pointer* data_ptr = vm->read(ptr);
    if (data_ptr == 0) {
        return;
    }
    if (data_ptr->type != TYPE_LIST) {
        return;
    }
    // ptr is already a valid address because of previous vm->read check
    data_ptr = vm->free(ptr);
    struct list_handler* handler = data_ptr->data;
    pointer->cleanup(&handler->list);
    list->destroy(&handler->list);
    global_free(data_ptr->data, data_ptr->size);
#ifdef USE_MEMORY_CLEANUP
    data_ptr->data = 0;
    data_ptr->size = 0;
#endif
    pointer->free(data_ptr);
}

static void list_push(u64 ptr_list, u64 ptr) {
    if (ptr_list == 0) {
        return;
    }
    if (ptr == 0) {
        return;
    }
    struct pointer* data_ptr = vm->read(ptr_list);
    if (data_ptr == 0) {
        return;
    }
    if (data_ptr->type != TYPE_LIST) {
        return;
    }
    if (data_ptr->data == 0) {
        return;
    }
    struct list_handler* handler = data_ptr->data;
    list->push(&handler->list, (void*)ptr);
#ifdef USE_GC
    list->push(&base->gc, (void*)ptr);
#endif
}

static u64 list_peek(u64 ptr) {
    if (ptr == 0) {
        return 0;
    }
    struct pointer* data_ptr = vm->read(ptr);
    if (data_ptr == 0) {
        return 0;
    }
    if (data_ptr->type != TYPE_LIST) {
        return 0;
    }
    if (data_ptr->data == 0) {
        return 0;
    }
    struct list_handler* handler = data_ptr->data;
    u64 data = (u64)list->peek(&handler->list);
    return data;
}

static u64 list_pop(u64 ptr) {
    if (ptr == 0) {
        return 0;
    }
    struct pointer* data_ptr = vm->read(ptr);
    if (data_ptr == 0) {
        return 0;
    }
    if (data_ptr->type != TYPE_LIST) {
        return 0;
    }
    if (data_ptr->data == 0) {
        return 0;
    }
    struct list_handler* handler = data_ptr->data;
    u64 data = (u64)list->pop(&handler->list);
    return data;
}

/* public */

void list_init() {
    pointer_vm_register_free(list_free);
}

const struct list_methods list_methods_definition = {
    .alloc = list_alloc,
    .free = list_free,
    .push = list_push,
    .peek = list_peek,
    .pop = list_pop
};
