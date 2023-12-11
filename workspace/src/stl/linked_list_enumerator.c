/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   11 December 2023 at 9:15:15 GMT+3
 *
 */
/*
    Copyright (C) 2022-2047 Artur Mustafin (artur.mustafin@gmail.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "stl/linked_list_enumerator.h"
#include "list/v1/list_v1.h"
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

static struct linked_list_enumerator* linked_list_enumerator_create(struct linked_list* list);
static void linked_list_enumerator_destroy(struct linked_list_enumerator** enumerator);
static void linked_list_enumerator_reverse(struct linked_list_enumerator* enumerator);
static struct linked_list_node* linked_list_enumerator_next(struct linked_list_enumerator* enumerator);
static struct linked_list_node* linked_list_enumerator_find(struct linked_list_enumerator* enumerator, void* data);
#ifdef USE_MEMORY_DEBUG_INFO
static void linked_list_print(struct linked_list* ptr);
#endif

/* implementation */
static struct linked_list_enumerator* linked_list_enumerator_create(struct linked_list* ptr) {
    if (!ptr) {
        return 0;
    }
    struct linked_list_enumerator* enumerator = calloc(1, sizeof(struct linked_list_enumerator));
    enumerator->list = ptr;
    enumerator->current = list->head(ptr);
    return enumerator;
}

static void linked_list_enumerator_destroy(struct linked_list_enumerator** enumerator) {
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
    .create = linked_list_enumerator_create,
    .destroy = linked_list_enumerator_destroy,
    .reverse = linked_list_enumerator_reverse,
    .next = linked_list_enumerator_next,
    .find = linked_list_enumerator_find,
    .find_prev = linked_list_enumerator_find_prev
};
