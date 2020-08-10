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

// default list methods
typedef struct list_vtable {
    // push item on current context (stack)
    void (*push)(list_context * const ctx, list_ptr item);
    // pop item on current context (stack)
    list_ptr (*pop)(list_context * const ctx);
    // peek item on current context (stack)
    list_ptr (*peek)(list_context * const ctx);
    // root item on current context (stack)
    list_ptr (*root)(list_context * const ctx);
    // free item on current context (stack)
    void (*free)(list_context * const ctx, list_ptr item);
    // alloc item on current context (stack)
    void (*alloc)(list_context * const ctx, abstract_ptr payload);
    // initialize context
    void (*init)(list_context * const ctx);
    // destroy context
    void (*destroy)(list_context * const ctx);
} list_vtable;

// static default implementation of null value for queue/struct
const static const list_ptr list_ptr_null;

// default list methods
void list_init(list_context * const ctx);
void list_alloc(list_context * const ctx, abstract_ptr payload);
void list_push(list_context * const ctx, list_ptr item);
list_ptr list_pop(list_context * const ctx);
list_ptr list_peek(list_context * const ctx);
list_ptr list_root(list_context * const ctx);
void list_free(list_context * const ctx, list_ptr item);
void list_destroy(list_context * const ctx);

// list vtable
static list_vtable list_vt = {
    .alloc = list_alloc,
    .push = list_push,
    .pop = list_pop,
    .peek = list_peek,
    .root = list_root,
    .free = list_free,
    .init = list_init,
    .destroy = list_destroy
};