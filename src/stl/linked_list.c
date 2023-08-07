/*
 *
 * MIT License
 *
 * Copyright (c) 2023 Artur Mustafin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */
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

/* definition */
extern const struct memory memory_definition;

/* definition */
static const struct memory* memory = &memory_definition;

/* implementation */

/* private */

static struct linked_list* linked_list_new(void);
static void linked_list_delete(struct linked_list** list);
static void linked_list_reverse(struct linked_list* list);

static void linked_list_reverse_until(struct linked_list* list, struct linked_list_node* node);
static void linked_list_reverse_until_match(struct linked_list* list, linked_list_node_match_function match);
static struct linked_list_node* linked_list_append_head(struct linked_list* list, void* data);
static struct linked_list_node* linked_list_append_tail(struct linked_list* list, void* data);
static struct linked_list_node* linked_list_head(struct linked_list* list);
static struct linked_list_node* linked_list_tail(struct linked_list* list);
static u64 linked_list_count(struct linked_list* list);
static void* linked_list_data(struct linked_list_node* node);
static struct linked_list_node* linked_list_next(struct linked_list_node* node);
static void linked_list_set_next(struct linked_list_node* node, struct linked_list_node* value);
static void linked_list_set(struct linked_list* list, struct linked_list_node* head, struct linked_list_node* tail);
#ifdef USE_MEMORY_DEBUG_INFO
static void linked_list_print(struct linked_list* list);
#endif

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
    struct linked_list_node* next = 0;
    while (node) {
        next = node->next;
        free(node);
        node = next;
    }
    ptr->head = 0;
    ptr->tail = 0;
    ptr->count = 0;
    free(ptr);
    *list = 0;
}

static void linked_list_reverse(struct linked_list* list) {
    if (!list || !list->head)
        return;

#ifdef USE_MEMORY_DEBUG_INFO
    linked_list_print(list);
#endif

    struct linked_list_node* current = list->head;

    struct linked_list_node* tmp;
    struct linked_list_node* prev = 0;
    struct linked_list_node* tail = current;

    while (current->next) {
        tmp = prev;
        prev = current;
        current = current->next;
        prev->next = tmp;
    }

    if (current) {
        current->next = prev;
    }

    list->tail = tail;
    list->head = current;

#ifdef USE_MEMORY_DEBUG_INFO
    linked_list_print(list);
#endif
}

static void linked_list_reverse_until(struct linked_list* list, struct linked_list_node* node) {
    if (!list || !node || !list->head)
        return;

    struct linked_list_node* current = list->head;

    if (current == node) {
        return;
    }

#ifdef USE_MEMORY_DEBUG_INFO
    linked_list_print(list);
#endif

    struct linked_list_node* tmp;
    struct linked_list_node* prev = 0;
    struct linked_list_node* tail = current;

    while (current->next) {
        tmp = prev;
        prev = current;
        current = current->next;
        prev->next = tmp;
        if (current == node) {
            if (current->next) {
                tail->next = current->next;
                tail = list->tail;
            }
            break;
        }
    }

    if (current)
        current->next = prev;

    list->tail = tail;
    list->head = current;

#ifdef USE_MEMORY_DEBUG_INFO
    linked_list_print(list);
#endif
}

static void linked_list_reverse_until_match(struct linked_list* list, linked_list_node_match_function match) {
    if (!list || !match || !list->head) {
        return;
    }

    struct linked_list_node* current = list->head;

    if (match(current)) {
        return;
    }

#ifdef USE_MEMORY_DEBUG_INFO
    linked_list_print(list);
#endif

    struct linked_list_node* tmp;
    struct linked_list_node* prev = 0;
    struct linked_list_node* tail = list->head;

    while (current->next) {
        tmp = prev;
        prev = current;
        current = current->next;
        prev->next = tmp;
        if (match(current)) {
            if (current->next != 0) {
                tail->next = current->next;
                tail = list->tail;
            }
            break;
        }
    }

    if (current) {
        current->next = prev;
    }

    list->tail = tail;
    list->head = current;

#ifdef USE_MEMORY_DEBUG_INFO
    linked_list_print(list);
#endif
}

#ifdef USE_MEMORY_DEBUG_INFO
static void linked_list_print(struct linked_list* list) {
    struct linked_list_node* head = list->head;
    struct linked_list_node* tail = list->tail;
    struct linked_list_node* current = head;
    printf("[");
    while (current) {
        printf("%lld", (u64)current->data);
        current = current->next;
        if (current) {
            printf(",");
        }
    }
    printf("] (%lld : %lld)\n", head ? (u64)head->data : 0, tail ? (u64)tail->data : 0);
}
#endif

static struct linked_list_node* linked_list_append_head(struct linked_list* list, void* data) {
    if (!list || !data) {
        return 0;
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

    return node;
}

static struct linked_list_node* linked_list_append_tail(struct linked_list* list, void* data) {
    if (!list || !data) {
        return 0;
    }

    struct linked_list_node* tail = list->tail;
    struct linked_list_node* node = calloc(1, sizeof(struct linked_list_node));

    node->data = data;
    if (tail) {
        tail->next = node;
    }
    list->tail = node;

    if (list->head == 0) {
        list->head = node;
    }

    list->count++;

    return node;
}

static struct linked_list_node* linked_list_head(struct linked_list* list) {
    if (!list) {
        return 0;
    }
    return list->head;
}

static struct linked_list_node* linked_list_tail(struct linked_list* list) {
    if (!list) {
        return 0;
    }
    return list->tail;
}

static u64 linked_list_count(struct linked_list* list) {
    if (!list) {
        return 0;
    }
    return list->count;
}

static void* linked_list_data(struct linked_list_node* node) {
    if (!node) {
        return 0;
    }
    return node->data;
}

static struct linked_list_node* linked_list_next(struct linked_list_node* node) {
    if (!node) {
        return 0;
    }
    return node->next;
}

static void linked_list_set_next(struct linked_list_node* node, struct linked_list_node* value) {
    if (!node || node == value) {
        return;
    }
    node->next = value;
}

static void linked_list_set(struct linked_list* list, struct linked_list_node* head, struct linked_list_node* tail) {
    if (!list || !head || !tail) {
        return;
    }
    list->head = head;
    list->tail = tail;
}

/* public */
const struct linked_list_methods linked_list_methods_definition = {
    /* generic methods */
    .new = linked_list_new,
    .delete = linked_list_delete,
    .append_head = linked_list_append_head,
    .append_tail = linked_list_append_tail,
    .reverse = linked_list_reverse,
    .reverse_until = linked_list_reverse_until,
    .reverse_until_match = linked_list_reverse_until_match,
    .count = linked_list_count,
    .data = linked_list_data,
    .next = linked_list_next,
    .head = linked_list_head,
    .tail = linked_list_tail,
    .set_next = linked_list_set_next,
    .set = linked_list_set,
};