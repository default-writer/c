#include <stdio.h>
#include <stdlib.h>
#define DEBUG

typedef struct q_type* q_type_ptr;
typedef void* payload_ptr;
typedef long long unsigned int LPADDR;

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
    if (head->prev == 0) {
        return 0;
    }
    q_type_ptr tmp = head;
    head = head->prev;
    head->next = 0;
    return tmp;
}

void list_alloc(payload_ptr payload) {
    q_type_ptr tmp = (q_type_ptr)malloc(sizeof(q_type));
    tmp->payload = payload;
    printf("alloc: 0x%llx 0x%llx\n", (LPADDR)tmp, (LPADDR)tmp->payload);
    q_push(tmp);
}

void list_print(q_type_ptr q_ptr) {
    int i = 0;
    while (q_ptr->prev != 0) {
        q_type_ptr tmp = q_ptr;
        q_ptr = q_ptr->prev;
        printf("%d: 0x%llx 0x%llx\n", ++i, (LPADDR)tmp, (LPADDR)tmp->payload);
    }
    printf("\n");
}

void list_free(q_type_ptr q_ptr) {
    while (q_ptr != 0) {
        q_type_ptr tmp = q_ptr;
        q_ptr = q_ptr->next;
        printf("free: 0x%llx 0x%llx\n", (LPADDR)tmp, (LPADDR)tmp->payload);
        free(tmp);
    }
    printf("\n");
}

int main() {
    head = (q_type_ptr)malloc(sizeof(q_type));
    payload_ptr payload = (payload_ptr)0xdeadbeef;
    list_alloc(payload);
    list_alloc(++payload);
    list_alloc(++payload);
    list_alloc(++payload);
    list_alloc(++payload);
    printf("\n");
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
    list_free(q_pop2);
#ifdef DEBUG
    list_print(head);
#endif
    q_type_ptr q_pop3 = q_pop(); 
    q_push(q_pop3);
    q_pop3 = q_pop(); 
    list_free(q_pop3);
#ifdef DEBUG
    list_print(head);
#endif
    q_type_ptr q_pop4 = q_pop(); 
    list_free(q_pop4);
#ifdef DEBUG
    list_print(head);
#endif
    free(head);
}