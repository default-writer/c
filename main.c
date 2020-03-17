#include <stdio.h>
#include <stdlib.h>

typedef struct q_type* q_type_ptr;
typedef void* payload_ptr;

typedef struct q_type {
    q_type_ptr next;
    q_type_ptr tail;
    payload_ptr payload;
} q_type;

q_type_ptr q_push(q_type_ptr queue, q_type_ptr q_ptr) {
    if (q_ptr != 0){
        q_ptr->next = queue->next;
        if (queue->next != 0) queue->next->tail = q_ptr;
        queue->next = q_ptr;
        if (queue->tail == 0) queue->tail = q_ptr;
    }
    return q_ptr;
}

q_type_ptr q_pop(q_type_ptr q_ptr) {
    if (q_ptr != 0) {
        q_type_ptr queue = q_ptr->tail;
        if (queue != 0 ) {
            q_ptr->tail = queue->tail;
            if (queue->tail != 0) queue->tail->next = 0;
            queue->tail = queue->next = 0;
            if (queue == q_ptr->next) q_ptr->next = 0;
        } else {
            q_ptr->tail = q_ptr->next = 0;
            return q_ptr;
        }
        return queue;
    }
    return 0;
}

q_type_ptr create_node(payload_ptr payload) {
    q_type_ptr q_ptr = (q_type_ptr)calloc(1, sizeof(q_type));
    q_ptr->payload = payload;
    return q_ptr;
}

void print_tree(q_type_ptr q_ptr) {
    while (q_ptr != 0) {
        if (q_ptr->payload != 0) {
            printf("0x%llx 0x%llx\n", q_ptr, q_ptr->payload);
        }
        q_ptr = q_ptr->tail;
    } 
    printf("\n");
}

void q_free(q_type_ptr q_ptr) {
    while (q_ptr != 0) {
        q_type_ptr next = q_ptr->tail;
        free(q_ptr);
        printf("0x%llx freed\n", q_ptr);
        q_ptr = next;
    }
    printf("\n");
}

int main() {
    payload_ptr payload = (payload_ptr)0xdeadbeef;

    q_type_ptr q_ptr0, q_ptr1, q_ptr2, q_ptr3, q_ptr4; //, q_ptr5, q_ptr6;
    q_type_ptr q_pop0, q_pop1, q_pop2, q_pop3, q_pop4;

    q_ptr0 = create_node(0);

    q_ptr1 = q_push(q_ptr0, create_node(payload++));
    q_ptr2 = q_push(q_ptr0, create_node(payload++));
    q_ptr3 = q_push(q_ptr0, create_node(payload++));
    q_ptr4 = q_push(q_ptr0, create_node(payload++));

    // q_ptr5 = q_push(q_ptr2, create_node(payload++)); //technically, insert 2 more points at the point q_ptr2, into queue  
    // q_ptr6 = q_push(q_ptr2, create_node(payload++)); 

    print_tree(q_ptr0);

    q_pop0 = q_pop(q_ptr0); 
    // q_pop1 = q_pop(q_ptr0);
    // q_pop2 = q_pop(q_ptr0);
    // q_pop3 = q_pop(q_ptr0);
    // q_pop4 = q_pop(q_ptr0);

    q_free(q_ptr0);
    q_free(q_pop0);
}