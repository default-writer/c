#ifndef _STL_LINKED_LIST_ENUMERATOR_H_
#define _STL_LINKED_LIST_ENUMERATOR_H_

#include "std/common.h"

struct linked_list;
struct linked_list_node;
struct linked_list_enumerator;

struct linked_list_enumerator_methods {
    struct linked_list_enumerator* (*new)(struct linked_list* list);
    void (*delete)(struct linked_list_enumerator** enumerator);
    struct linked_list_node* (*next)(struct linked_list_enumerator* enumerator);
};

#endif /* _STL_LINKED_LIST_ENUMERATOR_H_ */
