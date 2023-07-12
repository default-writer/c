#ifndef _STL_LINKED_LIST_H_
#define _STL_LINKED_LIST_H_

#include "std/common.h"

struct linked_list;
struct linked_list_node;

typedef u64 (*linked_list_node_match_function)(struct linked_list_node* current);

struct linked_list_methods {
    struct linked_list* (*new)(void);
    void (*delete)(struct linked_list** list);
    void (*reverse)(struct linked_list* list);
    void (*reverse_until)(struct linked_list* list, struct linked_list_node* node);
    void (*reverse_until_match)(struct linked_list* list, linked_list_node_match_function match);
    struct linked_list_node* (*append_head)(struct linked_list* list, void* data);
    struct linked_list_node* (*append_tail)(struct linked_list* list, void* data);
    u64 (*count)(struct linked_list* list);
    void* (*data)(struct linked_list_node* list);
    struct linked_list_node* (*next)(struct linked_list_node* node);
    struct linked_list_node* (*head)(struct linked_list* list);
    struct linked_list_node* (*tail)(struct linked_list* list);
    void (*set_next)(struct linked_list_node* node, struct linked_list_node* value);
    void (*set)(struct linked_list* list, struct linked_list_node* head, struct linked_list_node* tail);
};

#endif /* _STL_LINKED_LIST_H_ */
