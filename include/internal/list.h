#include <stdio.h>
#include <stdlib.h>

// abstract pointer type
typedef void* abstract_ptr;

// pointer abstraction on queue/list type
typedef struct q_type_ptr { 
    // pointer to queue/list
    struct q_type* ptr; 
} q_type_ptr;

// queue/list: pointer to prev, pointer to next, payload
typedef struct q_type {
    // points to previous (closer to root) node
    q_type_ptr prev;
    // points to next (farther from roon) node
    q_type_ptr next;
    // abstract payload
    abstract_ptr payload;
} q_type;

// pointer abstraction on queue/list type
typedef struct list_ptr { 
    // pointer to queue/list
    struct list* ptr; 
} list_ptr;

// queue/list context: root of the list, and element counter
typedef struct q_type_context { 
    // root element
    q_type_ptr head;
    // elements counter
    int count;
    // function pointer
    list_ptr f;
} q_type_context;
