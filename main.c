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
    void (*push)(q_type_ptr * const list, q_type_ptr* const next);
    q_type_ptr (*pop)(q_type_ptr * const list);
    void (*print)(const q_type_ptr const * const self);
    void (*free)(const q_type_ptr const * const self);
    void (*alloc)(q_type_ptr * const q, abstract_ptr payload);
} q_type;

void q_type_push(q_type_ptr * const list, q_type_ptr* const next);
q_type_ptr q_type_pop(q_type_ptr * const list);
void q_type_print(const q_type_ptr const * const self);
void q_type_free(const q_type_ptr const * const self);
void q_type_alloc(q_type_ptr * const q, abstract_ptr payload);

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

void q_type_alloc(q_type_ptr * const head, abstract_ptr payload) {
    q_type_ptr tmp;
    tmp.ptr = (q_type*)malloc(sizeof(q_type));
    tmp.ptr->payload = payload;
#ifdef DEBUG
    printf("alloc: 0x%llx 0x%llx\n", (ADDR)tmp.ptr, (ADDR)tmp.ptr->payload);
#endif
    q_type_push(head, &tmp);
}

q_type_ptr q_type_init() {
    q_type_ptr tmp;
    tmp.ptr = (q_type*)malloc(sizeof(q_type));
    return tmp;
}

typedef struct list_ptr {
    q_type_ptr head;
    int count;
    struct list *ptr;
} list_ptr;

typedef struct list {
    void (*push)(q_type_ptr * const list, q_type_ptr* const next);
    q_type_ptr (*pop)(q_type_ptr * const list);
    void (*print)(const q_type_ptr const * const self);
    void (*free)(const q_type_ptr const * const self);
    void (*alloc)(q_type_ptr * const q, abstract_ptr payload);
} list;

void list_q_type_push(list_ptr * const list, q_type_ptr* const next) {
    list->ptr->push(&(list->head), next);
}

q_type_ptr list_q_type_pop(list_ptr * const list) {
    return list->ptr->pop(&(list->head));
}

void list_q_type_print(list_ptr * const list) {
    list->ptr->print(&(list->head));
}

void list_q_type_free(list_ptr * const list, const q_type_ptr const * const q) {
    list->count--;
    list->ptr->free(q);
}

void list_q_type_alloc(list_ptr * const list, abstract_ptr payload) {
    list->ptr->alloc(&(list->head), payload);
    list->count++;
}

void list_free(list_ptr * const list) {
    list->ptr->free(&(list->head));
    free(list->ptr);
}

list_ptr list_init() {
    list_ptr tmp;
    tmp.ptr = (list*)malloc(sizeof(list));
    tmp.ptr->alloc = q_type_alloc;
    tmp.ptr->push = q_type_push;
    tmp.ptr->pop = q_type_pop;
    tmp.ptr->print = q_type_print;
    tmp.ptr->free = q_type_free;
    tmp.head = q_type_init();
    tmp.count = 0;
    return tmp;
}

void using_list_ptr(list_ptr head);

void using_list() {
    list_ptr head = list_init();
    using_list_ptr(head);
    list_free(&head);
}

void using_list_ptr(list_ptr head) {
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
}

int main() {
    using_list();
}