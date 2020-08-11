// abstract pointer type
typedef void* abstract_ptr;

const struct list_vtable list_vt;

// queue/list: pointer to prev, pointer to next, payload
struct list {
    // points to previous (closer to root) node
    struct list* prev;
    // points to next (farther from roon) node
    struct list* next;
    // abstract payload
    abstract_ptr payload;
};

// queue/list context: root of the list, and element counter
struct list_context { 
    // head element
    struct list* head;
    // root element
    struct list* root;
    // elements counter
    int count;
};

// default list methods
struct list_vtable {
    // push item on current context (stack)
    struct list* (*push)(struct list_context* const ctx, struct list** const item);
    // pop item on current context (stack)
    struct list* (*pop)(struct list_context* const ctx);
    // peek item on current context (stack)
    struct list* (*peek)(struct list_context* const ctx);
    // root item on current context (stack)
    struct list* (*root)(struct list_context* const ctx);
    // free item on current context (stack)
    void (*free)(struct list_context* const ctx, struct list** const item);
    // alloc item on current context (stack)
    void (*alloc)(struct list_context* const ctx, abstract_ptr payload);
    // initialize context
    void (*init)(struct list_context* const ctx);
    // destroy context
    void (*destroy)(struct list_context* const ctx);
};

// const default implementation of null value for queue/struct
static struct list* list_ptr_null;

