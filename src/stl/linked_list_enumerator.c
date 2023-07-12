#include "stl/linked_list_enumerator.h"
#include "list/data.h"
#include "stdlib.h"
#include "stl/linked_list.h"

/* public */
struct linked_list_enumerator {
    struct linked_list* list;
    struct linked_list_node* current;
};

/* definition */
extern const struct linked_list_methods linked_list_methods_definition;
static const struct linked_list_methods* list = &linked_list_methods_definition;

/* private */
static struct linked_list_enumerator* linked_list_enumerator_new(struct linked_list* list);
static void linked_list_enumerator_delete(struct linked_list_enumerator** enumerator);
static void linked_list_enumerator_reverse(struct linked_list_enumerator* enumerator);
static struct linked_list_node* linked_list_enumerator_next(struct linked_list_enumerator* enumerator);
static struct linked_list_node* linked_list_enumerator_find(struct linked_list_enumerator* enumerator, void* data);
#ifdef USE_MEMORY_DEBUG_INFO
static void linked_list_print(struct linked_list* ptr);
#endif

/* implementation */
static struct linked_list_enumerator* linked_list_enumerator_new(struct linked_list* ptr) {
    if (!ptr) {
        return 0;
    }
    struct linked_list_enumerator* enumerator = calloc(1, sizeof(struct linked_list_enumerator));
    enumerator->list = ptr;
    enumerator->current = list->head(ptr);
    return enumerator;
}

static void linked_list_enumerator_delete(struct linked_list_enumerator** enumerator) {
    if (!enumerator) {
        return;
    }
    struct linked_list_enumerator* ptr = *enumerator;
    free(ptr);
    *enumerator = 0;
}

static void linked_list_enumerator_reverse(struct linked_list_enumerator* enumerator) {
    if (!enumerator) {
        return;
    }

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

    list->set(ptr, current, tail);

#ifdef USE_MEMORY_DEBUG_INFO
    linked_list_print(ptr);
#endif
}

static struct linked_list_node* linked_list_enumerator_next(struct linked_list_enumerator* enumerator) {
    if (!enumerator || !enumerator->current) {
        return 0;
    }
    struct linked_list_node* node = enumerator->current;
    enumerator->current = list->next(node);
    return node;
}

static struct linked_list_node* linked_list_enumerator_find(struct linked_list_enumerator* enumerator, void* data) {
    if (!enumerator || !enumerator->current || !data) {
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

static struct linked_list_node* linked_list_enumerator_find_prev(struct linked_list_enumerator* enumerator, void* data) {
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
    if (ptr) {
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
}
#endif

/* public */
const struct linked_list_enumerator_methods linked_list_enumerator_methods_definition = {
    .new = linked_list_enumerator_new,
    .delete = linked_list_enumerator_delete,
    .reverse = linked_list_enumerator_reverse,
    .next = linked_list_enumerator_next,
    .find = linked_list_enumerator_find,
    .find_prev = linked_list_enumerator_find_prev
};
