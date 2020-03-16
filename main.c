#include <stdio.h>
#include <stdlib.h>

typedef struct q_type* q_type_ptr;
typedef void* payload_ptr;

typedef struct q_type {
    q_type_ptr next;
    q_type_ptr prev;
    payload_ptr payload;
} q_type;

q_type_ptr q_push(q_type_ptr queue, q_type_ptr q_ptr) {
    q_ptr->prev = queue->next;
    queue->next = q_ptr;
    return q_ptr;
}

q_type_ptr create_node(payload_ptr payload) {
    q_type_ptr q_ptr = (q_type_ptr)calloc(1, sizeof(q_type));
    q_ptr->next = 0;
    q_ptr->prev = 0;
    q_ptr->payload = payload;
    return q_ptr;
}

void print_tree(q_type_ptr q_ptr) {
    if (q_ptr != 0) {
        print_tree(q_ptr->prev);
        if (q_ptr->payload != 0) {
            printf("0x%llx\n", q_ptr->payload);
        }
        print_tree(q_ptr->next);
    }
}

void q_free(q_type_ptr q_ptr) {
    if (q_ptr != 0) {
        q_free(q_ptr->prev);       
        q_free_ptr(&(q_ptr->prev));
        q_free(q_ptr->next);
        q_free_ptr(&(q_ptr->next));
    }
}

void q_free_ptr(q_type_ptr *q_ptr_addr) {
    if (*q_ptr_addr != 0) {
        free(*q_ptr_addr);
        printf("0x%llx freed\n", *q_ptr_addr);
        *q_ptr_addr = 0;
    }
}

int main() {
    payload_ptr payload = (payload_ptr)0xdeadbeef;
    q_type_ptr q_ptr0, q_ptr1, q_ptr2, q_ptr3, q_ptr4, q_ptr5, q_ptr6;
    
    q_ptr0 = create_node(0);

    q_ptr1 = q_push(q_ptr0, create_node(payload++));
    q_ptr2 = q_push(q_ptr0, create_node(payload++));
    q_ptr3 = q_push(q_ptr0, create_node(payload++));
    q_ptr4 = q_push(q_ptr0, create_node(payload++));

    q_ptr5 = q_push(q_ptr2, create_node(payload++));    
    q_ptr6 = q_push(q_ptr2, create_node(payload++));

    print_tree(q_ptr0);

    q_free(q_ptr0);

    q_free_ptr(&q_ptr0);
}