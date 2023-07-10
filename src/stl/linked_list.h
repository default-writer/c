#ifndef _STL_LINKED_LIST_H_
#define _STL_LINKED_LIST_H_

#include "std/common.h"

struct linked_list;
struct linked_list_node;
struct linked_list_enumerator;

struct linked_list_methods {
    struct linked_list* (*new)(void);
    void (*delete)(struct linked_list** list);
    void (*reverse)(struct linked_list* list);
    void (*reverse_until)(struct linked_list* list, struct linked_list_node* node);
    void (*append_head)(struct linked_list* list, void* data);
    void (*append_tail)(struct linked_list* list, void* data);
    u64 (*count)(struct linked_list* list);
    void* (*data)(struct linked_list_node* list);
    struct linked_list_node* (*next)(struct linked_list_node* node);
    struct linked_list_node* (*head)(struct linked_list* list);
    struct linked_list_node* (*tail)(struct linked_list* list);
};

#endif /* _STL_LINKED_LIST_H_ */
