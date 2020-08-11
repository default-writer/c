// queue/list: vtable
const struct list_vtable list_vt;

// queue/list: prev, next, payload
struct list {
    // points to previous (closer to root) node
    struct list* prev;
    // points to next (farther from roon) node
    struct list* next;
    // payload
    void*  payload;
};

// queue/list context: root, head, element counter
struct list_context { 
    // root element
    struct list* root;
    // head element
    struct list* head;
    // elements counter
    int count;
};

// queue/list: vtable definition
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
    void (*alloc)(struct list_context* const ctx, void* payload);
    // initialize context
    void (*init)(struct list_context* const ctx);
    // destroy context
    void (*destroy)(struct list_context* const ctx);
};

// static default implementation of null value for queue/struct
 static struct list* list_ptr_null;
 