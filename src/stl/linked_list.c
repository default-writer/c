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

/* private */
static struct linked_list* linked_list_new(void);
static void linked_list_delete(struct linked_list** list);
static void linked_list_reverse_list(struct linked_list* list);
static void linked_list_append_head(struct linked_list* list, void* data);
static void linked_list_append_tail(struct linked_list* list, void* data);
static struct linked_list_node* linked_list_head(struct linked_list* list);
static struct linked_list_node* linked_list_tail(struct linked_list* list);
static u64 linked_list_count(struct linked_list* list);
static void* linked_list_data(struct linked_list_node* node);
static struct linked_list_node* linked_list_next(struct linked_list_node* node);

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
    if (tail != 0) {
        tail->next = node;
    }
    list->tail = node;
    if (list->head == 0) {
        list->head = node;
    }
    list->count++;
}

static struct linked_list_node* linked_list_head(struct linked_list* list) {
    if (list == 0) {
        return 0;
    }
    return list->head;
}

static struct linked_list_node* linked_list_tail(struct linked_list* list) {
    if (list == 0) {
        return 0;
    }
    return list->tail;
}

static u64 linked_list_count(struct linked_list* list) {
    if (list == 0) {
        return 0;
    }
    return list->count;
}

static void* linked_list_data(struct linked_list_node* node) {
    if (node == 0) {
        return 0;
    }
    return node->data;
}

static struct linked_list_node* linked_list_next(struct linked_list_node* node) {
    if (node == 0) {
        return 0;
    }
    return node->next;
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
    .next = linked_list_next,
    .head = linked_list_head,
    .tail = linked_list_tail
};
