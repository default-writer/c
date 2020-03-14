#include <stdio.h>
#include <stdlib.h>




typedef struct q_type* q_type_ptr;

typedef struct queue* queue_ptr;
typedef void* payload_ptr;

typedef struct queue {
    q_type_ptr tail;
    q_type_ptr head;
    uint length;
}queue;

typedef struct q_type {
    q_type_ptr next;
    q_type_ptr prev;
    payload_ptr payload;
}q_type ;


q_type_ptr q_push(queue_ptr queue, payload_ptr payload) {
    q_type_ptr q_ptr = (q_type_ptr)calloc(1, sizeof(q_type));
    q_ptr->payload = payload;
    q_ptr->prev = queue->tail;
    if (queue->length > 0) {
        queue->tail->next = q_ptr; 
        queue->tail = q_ptr;
    } else {
        queue->head = queue->tail = q_ptr;
    }
    queue->length += 1;
    return q_ptr;
}

queue_ptr create_queue() {
    queue_ptr queue = (queue_ptr)calloc(1, sizeof(queue));
    return queue;
}

int main() {
    payload_ptr payload = (payload_ptr)0xDEADBEEF;
    queue_ptr queue = create_queue();
    q_type_ptr q_ptr;
    q_ptr = q_push(queue, payload);
    q_ptr = q_push(queue, payload);
    q_ptr = q_push(queue, payload);
    q_ptr = q_push(queue, payload);
    q_ptr = q_push(queue, payload);
    q_ptr = q_push(queue, payload);
    q_ptr = q_push(queue, payload);
}