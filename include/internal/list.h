#include <stdio.h>
#include <stdlib.h>

// abstract pointer type
typedef void* abstract_ptr;

// pointer abstraction on queue/list type
typedef struct list_ptr { 
    // pointer to queue/list
    struct list* ptr; 
} list_ptr;

// queue/list: pointer to prev, pointer to next, payload
typedef struct list {
    // points to previous (closer to root) node
    list_ptr prev;
    // points to next (farther from roon) node
    list_ptr next;
    // abstract payload
    abstract_ptr payload;
} list;

// queue/list context: root of the list, and element counter
typedef struct list_context { 
    // head element
    list_ptr head;
    // root element
    list_ptr root;
    // elements counter
    int count;
} list_context;
