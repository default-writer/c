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

// queue/list context: root of the list, and element counter
typedef struct q_type_context { 
    // root element
    q_type_ptr head;
    // elements counter
    int count;
} q_type_context;

// queue/list: current context structure, and methods to operate on it
typedef struct list {
    // current context (stack)
    q_type_context context;
    // push item on current context (stack)
    void (*push)(q_type_context * const ctx, q_type_ptr * const item);
    // poph item on current context (stack)
    q_type_ptr (*pop)(q_type_context * const ctx);
    // print item on current context (stack)
    void (*print)(q_type_context * const ctx);
    // free item on current context (stack)
    void (*free)(q_type_context * const ctx, q_type_ptr * const item);
    // alloc item on current context (stack)
    void (*alloc)(q_type_context * const ctx, abstract_ptr payload);
} list;