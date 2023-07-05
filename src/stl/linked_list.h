#ifndef _STL_LINKED_LIST_H_
#define _STL_LINKED_LIST_H_

struct linked_list {
    struct linked_list* next;
    void* data;
};

struct linked_list_methods {
    struct linked_list* (*new)(void);
    void (*delete)(struct linked_list* head);
    struct linked_list* (*reverse_list)(struct linked_list* head);
};

#endif /* _STL_LINKED_LIST_H_ */
