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
 
const static const q_type_ptr const q_type_ptr_null;

void q_type_push(q_type_ptr * const head, q_type_ptr* const next) {
    head->ptr->next.ptr = next->ptr;
    next->ptr->prev.ptr = head->ptr;
    head->ptr = next->ptr;
}

q_type_ptr q_type_pop(q_type_ptr * const head) {
    q_type_ptr tmp;
    if (head->ptr->prev.ptr == 0) {
        return q_type_ptr_null;
    }
    tmp.ptr = head->ptr;
    head->ptr = head->ptr->prev.ptr;
    head->ptr->next.ptr = 0;
    return tmp;
}

void q_type_alloc(q_type_ptr * const head, abstract_ptr payload) {
    q_type_ptr tmp;
    tmp.ptr = (q_type*)malloc(sizeof(q_type));
    tmp.ptr->payload = payload;
#ifdef DEBUG
    printf("alloc: 0x%llx 0x%llx\n", (ADDR)tmp.ptr, (ADDR)tmp.ptr->payload);
#endif
    q_type_push(head, &tmp);
}

void q_type_print(const q_type_ptr const * const q_ptr) {
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

void q_type_free(const q_type_ptr const * const q_ptr) {
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

typedef struct list_ptr {
    struct list *ptr;
} list_ptr;

typedef struct list {
    q_type_ptr head;
    int count;
    void (*q_type_alloc)(q_type_ptr * const q, abstract_ptr payload);
    void (*q_type_push)(q_type_ptr * const list, q_type_ptr* const next);
    q_type_ptr (*q_type_pop)(q_type_ptr * const list);
    void (*q_type_print)(const q_type_ptr const * const self);
    void (*q_type_free)(const q_type_ptr const * const self);
} list;

list_ptr list_alloc() {
    list_ptr tmp;
    tmp.ptr = (list*)malloc(sizeof(list));
    tmp.ptr->head.ptr = (q_type*)malloc(sizeof(q_type));
    tmp.ptr->q_type_alloc = q_type_alloc;
    tmp.ptr->q_type_push = q_type_push;
    tmp.ptr->q_type_pop = q_type_pop;
    tmp.ptr->q_type_print = q_type_print;
    tmp.ptr->q_type_free = q_type_free;
    return tmp;
}

void list_q_type_push(const list_ptr const * const list, q_type_ptr* const next) {
    list->ptr->q_type_push(&(list->ptr->head), next);
}

q_type_ptr list_q_type_pop(const list_ptr const * const list) {
    return list->ptr->q_type_pop(&(list->ptr->head));
}

void list_q_type_alloc(const list_ptr const * const list, abstract_ptr payload) {
    list->ptr->q_type_alloc(&(list->ptr->head), payload);
    list->ptr->count++;
}

void list_q_type_print(const list_ptr const * const list) {
    list->ptr->q_type_print(&(list->ptr->head));
}

void list_q_type_free(const list_ptr const * const list, const q_type_ptr const * const q) {
    list->ptr->count--;
    list->ptr->q_type_free(q);
}

void list_free(const list_ptr const * const list) {
    free(list->ptr->head.ptr);
    free(list->ptr);
}

int main() {
    list_ptr head = list_alloc();
    abstract_ptr payload = (abstract_ptr)0xdeadbeef;
    list_q_type_alloc(&head, payload);
    list_q_type_alloc(&head, ++payload);
    list_q_type_alloc(&head, ++payload);
    list_q_type_alloc(&head, ++payload);
    list_q_type_alloc(&head, ++payload);
#ifdef DEBUG
    printf("\n");
#endif
#ifdef DEBUG
    list_q_type_print(&head);
#endif
    q_type_ptr q_pop0 = list_q_type_pop(&head); 
    list_q_type_free(&head, &q_pop0);
#ifdef DEBUG
    list_q_type_print(&head);
#endif
    q_type_ptr q_pop1 = list_q_type_pop(&head); 
    list_q_type_free(&head, &q_pop1);
#ifdef DEBUG
    list_q_type_print(&head);
#endif
    q_type_ptr q_pop2 = list_q_type_pop(&head); 
    list_q_type_free(&head, &q_pop2);
#ifdef DEBUG
    list_q_type_print(&head);
#endif
    q_type_ptr q_pop3 = list_q_type_pop(&head); 
    list_q_type_push(&head, &q_pop3);
    q_pop3 = list_q_type_pop(&head); 
    list_q_type_free(&head, &q_pop3);
#ifdef DEBUG
    list_q_type_print(&head);
#endif
    q_type_ptr q_pop4 = list_q_type_pop(&head); 
    list_q_type_free(&head, &q_pop4);
#ifdef DEBUG
    list_q_type_print(&head);
#endif
    q_type_ptr q_pop5 = list_q_type_pop(&head); 
    list_q_type_free(&head, &q_pop5);
#ifdef DEBUG
    list_q_type_print(&head);
#endif
    list_free(&head);
}