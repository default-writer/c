#include <stdio.h>
#include <stdlib.h>

typedef struct q_type* q_type_ptr;
typedef void* payload_ptr;

typedef struct q_type {
    q_type_ptr next;
    q_type_ptr prev;
    payload_ptr payload;
} q_type;

q_type_ptr q_push(q_type_ptr queue, payload_ptr payload) {
    if (payload == -1) return -1;
    q_type_ptr q_ptr = (q_type_ptr)calloc(1, sizeof(q_type));
    q_ptr->payload = payload;
    if (queue->payload++ > 0) {
        q_ptr->prev = queue->next;
        queue->next->next = q_ptr; 
        queue->next = q_ptr;
    } else {
        queue->prev = queue->next = q_ptr;
    }
    return q_ptr;
}

q_type_ptr q_pop(q_type_ptr queue) {
    if (queue->payload == 0) return -1;
    q_type_ptr q_ptr = queue->next;
    if (--queue->payload > 0) {
        queue->next = queue->next->prev;
        queue->next->next = 0;
        q_ptr->prev = 0;
    } else {
        queue->prev = queue->next = 0;
    }
    return q_ptr;
}

q_type_ptr create_queue() {
    q_type_ptr queue = (q_type_ptr)calloc(1, sizeof(queue));
    return queue;
}

int main() {
    payload_ptr payload = (payload_ptr)0xdeadbeef;
    q_type_ptr q_ptr = create_queue();
    q_type_ptr q_ptr1, q_ptr2, q_ptr3, q_ptr4;
    q_ptr1 = q_push(q_ptr, payload++);
    q_ptr2 = q_push(q_ptr, payload++);
    q_ptr3 = q_push(q_ptr, payload++);
    q_ptr4 = q_push(q_ptr, payload++);
    q_ptr1 = q_pop(q_ptr);
    q_ptr2 = q_pop(q_ptr);
    q_ptr3 = q_pop(q_ptr);
    q_ptr4 = q_pop(q_ptr);
    free(q_ptr1);
    free(q_ptr2);
    free(q_ptr3);
    free(q_ptr4);
    free(q_ptr);
}