#include "stl/linked_list_static_enumerator.h"
#include "list/data.h"
#include "stdlib.h"
#include "stl/linked_list.h"

/* public */
struct linked_list_enumerator_data {
    struct linked_list* list;
    struct linked_list_node* current;
};

/* definition */
extern const struct linked_list_methods linked_list_methods_definition;
static const struct linked_list_methods* list = &linked_list_methods_definition;
static struct linked_list_enumerator_data static_linked_list_enumerator_data;
static struct linked_list_enumerator_data* enumerator = &static_linked_list_enumerator_data;

/* private */
static void linked_list_enumerator_init(struct linked_list* list);
static void linked_list_enumerator_destroy(void);
static struct linked_list_node* linked_list_enumerator_next(void);
static struct linked_list_node* linked_list_enumerator_find(void* data);
static void linked_list_enumerator_reverse(void);
static struct linked_list_node* linked_list_enumerator_find_prev(void* data);
#ifdef USE_MEMORY_DEBUG_INFO
static void linked_list_print(struct linked_list* ptr);
#endif

/* implementation */
static void linked_list_enumerator_init(struct linked_list* ptr) {
    if (!ptr) {
        return;
    }
    enumerator->list = ptr;
    enumerator->current = list->head(ptr);
}

static void linked_list_enumerator_destroy(void) {
    enumerator->list = 0;
    enumerator->current = 0;
}

static void linked_list_enumerator_reverse(void) {
    struct linked_list* ptr = enumerator->list;
    if (!ptr || !list->head(ptr))
        return;

#ifdef USE_MEMORY_DEBUG_INFO
    linked_list_print(ptr);
#endif

    struct linked_list_node* current = list->head(ptr);

    struct linked_list_node* tmp;
    struct linked_list_node* prev = 0;
    struct linked_list_node* tail = current;

    while (list->next(current)) {
        tmp = prev;
        prev = current;
        current = list->next(current);
        list->set_next(prev, tmp);
    }

    if (current) {
        list->set_next(current, prev);
    }

    list->set_next(tail, 0);
    list->set(ptr, current, tail);

#ifdef USE_MEMORY_DEBUG_INFO
    linked_list_print(ptr);
#endif
}

static struct linked_list_node* linked_list_enumerator_next(void) {
    if (!enumerator->current) {
        return 0;
    }
    struct linked_list_node* node = enumerator->current;
    enumerator->current = list->next(node);
    return node;
}

static struct linked_list_node* linked_list_enumerator_find(void* data) {
    if (!enumerator->current || !data) {
        return 0;
    }
    struct linked_list_node* node = enumerator->current;
    while (node) {
        if (list->data(node) == data) {
            break;
        }
        node = list->next(node);
    }
    enumerator->current = list->next(node);
    return node;
}

static struct linked_list_node* linked_list_enumerator_find_prev(void* data) {
    if (!enumerator || !enumerator->current || !data) {
        return 0;
    }
    struct linked_list_node* node = enumerator->current;
    struct linked_list_node* prev = 0;
    while (node) {
        if (list->data(node) == data) {
            break;
        }
        prev = node;
        node = list->next(node);
    }
    enumerator->current = list->next(node);
    return prev;
}

#ifdef USE_MEMORY_DEBUG_INFO
static void linked_list_print(struct linked_list* ptr) {
    struct linked_list_node* head = list->head(ptr);
    struct linked_list_node* tail = list->tail(ptr);
    struct linked_list_node* current = head;
    printf("[");
    while (current) {
        printf("%lld", (u64)list->data(current));
        current = list->next(current);
        if (current) {
            printf(",");
        }
    }
    printf("] (%lld : %lld)\n", head ? (u64)list->data(head) : 0, tail ? (u64)list->data(tail) : 0);
}
#endif

/* public */
const struct linked_list_static_enumerator_methods linked_list_static_enumerator_methods_definition = {
    .init = linked_list_enumerator_init,
    .destroy = linked_list_enumerator_destroy,
    .reverse = linked_list_enumerator_reverse,
    .next = linked_list_enumerator_next,
    .find = linked_list_enumerator_find,
    .find_prev = linked_list_enumerator_find_prev
};
