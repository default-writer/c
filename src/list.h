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
    // struct list* root;
    // head element
    struct list* head;
    // elements counter
    //int count;
};
