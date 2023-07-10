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
static struct linked_list_node* linked_list_enumerator_next(struct linked_list_enumerator* enumerator);
static struct linked_list_node* linked_list_enumerator_find(struct linked_list_enumerator* enumerator, void* data);
static struct linked_list_node* linked_list_enumerator_find_prev(struct linked_list_enumerator* enumerator, void* data);

/* implementation */
static struct linked_list_enumerator* linked_list_enumerator_new(struct linked_list* ptr) {
    if (ptr == 0) {
        return 0;
    }
    struct linked_list_enumerator* enumerator = calloc(1, sizeof(struct linked_list_enumerator));
    enumerator->list = ptr;
    enumerator->current = list->head(ptr);
    return enumerator;
}

static void linked_list_enumerator_delete(struct linked_list_enumerator** enumerator) {
    if (enumerator == 0) {
        return;
    }
    struct linked_list_enumerator* ptr = *enumerator;
    free(ptr);
    *enumerator = 0;
}

static struct linked_list_node* linked_list_enumerator_next(struct linked_list_enumerator* enumerator) {
    if (enumerator == 0) {
        return 0;
    }
    if (enumerator->current == 0) {
        return 0;
    }
    struct linked_list_node* node = enumerator->current;
    enumerator->current = list->next(node);
    return node;
}

static struct linked_list_node* linked_list_enumerator_find(struct linked_list_enumerator* enumerator, void* data) {
    if (enumerator == 0) {
        return 0;
    }
    if (enumerator->current == 0) {
        return 0;
    }
    if (data == 0) {
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
    if (enumerator == 0) {
        return 0;
    }
    if (enumerator->current == 0) {
        return 0;
    }
    if (data == 0) {
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

/* public */
const struct linked_list_enumerator_methods linked_list_enumerator_methods_definition = {
    .new = linked_list_enumerator_new,
    .delete = linked_list_enumerator_delete,
    .next = linked_list_enumerator_next,
    .find = linked_list_enumerator_find,
    .find_prev = linked_list_enumerator_find_prev
};
