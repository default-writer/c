#ifndef _STL_LINKED_LIST_STATIC_ENUMERATOR_H_
#define _STL_LINKED_LIST_STATIC_ENUMERATOR_H_

#include "std/common.h"

struct linked_list;
struct linked_list_node;

struct linked_list_static_enumerator_methods {
    void (*init)(struct linked_list* list);
    void (*destroy)(void);
    void (*reverse)(void);
    struct linked_list_node* (*next)(void);
    struct linked_list_node* (*find)(void* data);
    struct linked_list_node* (*find_prev)(void* data);
};

#endif /* _STL_LINKED_LIST_ENUMERATOR_H_ */
