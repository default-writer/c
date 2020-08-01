#include <stdio.h>
#include <stdlib.h>
#define DEBUG

typedef struct q_type* q_type_ptr;
typedef void* payload_ptr;

typedef struct q_type {
    q_type_ptr prev;
    q_type_ptr next;
    payload_ptr payload;
} q_type;

q_type_ptr head;

void q_push(q_type_ptr next) {
    head->next = next;
    next->prev = head;
    head = next;
}
q_type_ptr q_pop() {
    q_type_ptr tmp = head;
    q_type_ptr next = head->prev;
    head = next;
    head->next = 0;
    tmp->prev = 0;
    return tmp;
}

void list_create(payload_ptr payload) {
    q_type_ptr q_ptr = (q_type_ptr)malloc(sizeof(q_type));
    q_ptr->payload = payload;
    q_push(q_ptr);
}

void list_print(q_type_ptr q_ptr) {
    if (q_ptr != 0) {
        q_type_ptr head = q_ptr;
        while (q_ptr->prev != 0) {
            printf("0x%llx 0x%llx\n", q_ptr, q_ptr->payload);
            q_ptr = q_ptr->prev;
        } 
    }
    printf("\n");
}

void list_free(q_type_ptr q_ptr) {
    while (q_ptr != 0) {
        q_type_ptr prev = q_ptr->next;
        free(q_ptr);
        q_ptr->next = 0;
        q_ptr->payload = 0;
        q_ptr = prev;
    }
    printf("\n");
}

int main() {
    head = (q_type_ptr)malloc(sizeof(q_type));
    payload_ptr payload = (payload_ptr)0xdeadbeef;
    list_create(payload);
    list_create(payload++);
    list_create(payload++);
    list_create(payload++);
    list_create(payload++);
#ifdef DEBUG
    list_print(head);
#endif
    q_type_ptr q_pop0 = q_pop(); 
    list_free(q_pop0);
#ifdef DEBUG
    list_print(head);
#endif
    q_type_ptr q_pop1 = q_pop(); 
    list_free(q_pop1);
#ifdef DEBUG
    list_print(head);
#endif
    q_type_ptr q_pop2 = q_pop(); 
    list_print(head);
#ifdef DEBUG
    list_print(q_pop2);
#endif
    q_type_ptr q_pop3 = q_pop(); 
    list_free(q_pop3);
#ifdef DEBUG
    list_print(head);
#endif
    q_type_ptr q_pop4 = q_pop(); 
    list_free(q_pop4);
#ifdef DEBUG
    list_print(head);
#endif
    q_type_ptr q_pop5 = q_pop(); 
    list_free(q_pop5);
#ifdef DEBUG
    list_print(head);
#endif    
    q_type_ptr q_pop6 = q_pop(); 
    list_free(q_pop6);
#ifdef DEBUG
    list_print(head);
#endif
    free(head);
}