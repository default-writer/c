typedef long long unsigned int ADDR;
typedef void* abstract_ptr;

typedef struct q_type_ptr { 
    struct q_type* ptr; 
} q_type_ptr;

typedef struct q_type {
    q_type_ptr prev;
    q_type_ptr next;
    abstract_ptr payload;
} q_type;

typedef struct q_type_context { 
    q_type_ptr head;
    int count;
} q_type_context;

