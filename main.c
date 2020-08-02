#include <stdio.h>
#include <stdlib.h>
#define DEBUG

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
 

void q_push(q_type_ptr * const head, q_type_ptr* const next) {
    head->ptr->next.ptr = next->ptr;
    next->ptr->prev.ptr = head->ptr;
    head->ptr = next->ptr;
}

q_type_ptr q_pop(q_type_ptr * const head) {
    q_type_ptr tmp;
    if (head->ptr->prev.ptr == 0) {
        return head->ptr->prev;
    }
    tmp.ptr = head->ptr;
    head->ptr = head->ptr->prev.ptr;
    head->ptr->next.ptr = 0;
    return tmp;
}

void list_alloc(q_type_ptr * const head, abstract_ptr payload) {
    q_type_ptr tmp;
    tmp.ptr = (q_type*)malloc(sizeof(q_type));
    tmp.ptr->payload = payload;
#ifdef DEBUG
    printf("alloc: 0x%llx 0x%llx\n", (ADDR)tmp.ptr, (ADDR)tmp.ptr->payload);
#endif
    q_push(head, &tmp);
}

void list_print(const q_type_ptr const * const q_ptr) {
    int i = 0;
    q_type_ptr tmp;
    tmp.ptr = q_ptr->ptr;
    while (tmp.ptr->prev.ptr != 0) {
#ifdef DEBUG
        printf("%d: 0x%llx 0x%llx\n", ++i, (ADDR)tmp.ptr, (ADDR)tmp.ptr->payload);
#endif
        tmp.ptr = tmp.ptr->prev.ptr;
    }
#ifdef DEBUG
    printf("\n");
#endif
}

void list_free(const q_type_ptr const * const q_ptr) {
    q_type_ptr tmp;
    tmp.ptr = q_ptr->ptr;
    while (tmp.ptr != 0) {
#ifdef DEBUG
        printf("free: 0x%llx 0x%llx\n", (ADDR)tmp.ptr, (ADDR)tmp.ptr->payload);
#endif
        q_type* ptr = tmp.ptr;
        tmp.ptr = tmp.ptr->next.ptr;
        free(ptr);
    }
#ifdef DEBUG
    printf("\n");
#endif
}

int main() {
    q_type_ptr head;
    head.ptr = (q_type*)malloc(sizeof(q_type));
    abstract_ptr payload = (abstract_ptr)0xdeadbeef;
    list_alloc(&head, payload);
    list_alloc(&head, ++payload);
    list_alloc(&head, ++payload);
    list_alloc(&head, ++payload);
    list_alloc(&head, ++payload);
#ifdef DEBUG
    printf("\n");
#endif
#ifdef DEBUG
    list_print(&head);
#endif
    q_type_ptr q_pop0 = q_pop(&head); 
    list_free(&q_pop0);
#ifdef DEBUG
    list_print(&head);
#endif
    q_type_ptr q_pop1 = q_pop(&head); 
    list_free(&q_pop1);
#ifdef DEBUG
    list_print(&head);
#endif
    q_type_ptr q_pop2 = q_pop(&head); 
    list_free(&q_pop2);
#ifdef DEBUG
    list_print(&head);
#endif
    q_type_ptr q_pop3 = q_pop(&head); 
    q_push(&head, &q_pop3);
    q_pop3 = q_pop(&head); 
    list_free(&q_pop3);
#ifdef DEBUG
    list_print(&head);
#endif
    q_type_ptr q_pop4 = q_pop(&head); 
    list_free(&q_pop4);
#ifdef DEBUG
    list_print(&head);
#endif
    q_type_ptr q_pop5 = q_pop(&head); 
    list_free(&q_pop5);
#ifdef DEBUG
    list_print(&head);
#endif
    free(head.ptr);
}