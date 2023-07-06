#include "stl/linked_list.h"
#include "list/data.h"
#include "stdlib.h"

struct linked_list_node {
    struct linked_list_node* next;
    void* data;
};

struct linked_list {
    struct linked_list_node* head;
    struct linked_list_node* tail;
    u64 count;
};

struct linked_list_enumerator {
    struct linked_list* list;
    struct linked_list_node* current;
};

/* private */
static struct linked_list* linked_list_new(void);
static void linked_list_delete(struct linked_list** list);
static void linked_list_reverse_list(struct linked_list* list);
static void linked_list_append_head(struct linked_list* list, void* data);
static void linked_list_append_tail(struct linked_list* list, void* data);
static u64 linked_list_count(struct linked_list* list);
static void* linked_list_data(struct linked_list_node* node);
static struct linked_list_node* linked_list_next(struct linked_list_node* node);

static struct linked_list_enumerator* linked_list_enumerator_new(struct linked_list* list);
static void linked_list_enumerator_delete(struct linked_list_enumerator** enumerator);
static struct linked_list_node* linked_list_enumerator_next(struct linked_list_enumerator*);

/* implementation */
static struct linked_list* linked_list_new(void) {
    struct linked_list* list = calloc(1, sizeof(struct linked_list));
    return list;
}

static void linked_list_delete(struct linked_list** list) {
    if (list == 0) {
        return;
    }
    struct linked_list* ptr = *list;
    struct linked_list_node* node = ptr->head;
    while (node != 0) {
        struct linked_list_node* next = node->next;
        free(node);
        node = next;
    }
    ptr->head = 0;
    ptr->tail = 0;
    ptr->count = 0;
    free(ptr);
    *list = 0;
}

static void linked_list_reverse_list(struct linked_list* list) {
    if (list == 0) {
        return;
    }
    struct linked_list_node* current = list->head;
    struct linked_list_node* prev = 0;
    struct linked_list_node* tmp = 0;
    while (current) {
        tmp = prev;
        prev = current;
        current = current->next;
        prev->next = tmp;
    }
    list->tail = list->head;
    list->head = prev;
}

static void linked_list_append_head(struct linked_list* list, void* data) {
    if (list == 0) {
        return;
    }
    if (data == 0) {
        return;
    }
    struct linked_list_node* head = list->head;
    struct linked_list_node* node = calloc(1, sizeof(struct linked_list_node));
    node->data = data;
    node->next = head;
    list->head = node;
    if (list->tail == 0) {
        list->tail = node;
    }
    list->count++;
}

static void linked_list_append_tail(struct linked_list* list, void* data) {
    if (list == 0) {
        return;
    }
    if (data == 0) {
        return;
    }
    struct linked_list_node* tail = list->tail;
    struct linked_list_node* node = calloc(1, sizeof(struct linked_list_node));
    node->data = data;
    tail->next = node;
    list->tail = node;
    if (list->head == 0) {
        list->head = node;
    }
    list->count++;
}

static u64 linked_list_count(struct linked_list* list) {
    if (list == 0) {
        return 0;
    }
    return list->count;
}

static void* linked_list_data(struct linked_list_node* node) {
    return node->data;
}

static struct linked_list_node* linked_list_next(struct linked_list_node* node) {
    return node->next;
}

static struct linked_list_enumerator* linked_list_enumerator_new(struct linked_list* list) {
    if (list == 0) {
        return 0;
    }
    struct linked_list_enumerator* enumerator = calloc(1, sizeof(struct linked_list_enumerator));
    enumerator->list = list;
    enumerator->current = list->head;
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
    enumerator->current = node->next;
    return node;
}

/* public */

const struct linked_list_methods linked_list_methods_definition = {
    /* generic methods */
    .new = linked_list_new,
    .delete = linked_list_delete,
    .append_head = linked_list_append_head,
    .append_tail = linked_list_append_tail,
    .reverse_list = linked_list_reverse_list,
    .count = linked_list_count,
    .data = linked_list_data,
    .next = linked_list_next
};

const struct linked_list_enumerator_methods linked_list_enumerator_methods_definition = {
    .new = linked_list_enumerator_new,
    .delete = linked_list_enumerator_delete,
    .next = linked_list_enumerator_next
};
