#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#define DEBUG

const static const q_type_ptr const q_type_ptr_null;

void q_type_push(q_type_context * const ctx, q_type_ptr* const item) {
    q_type_ptr* head = &(ctx->head);
    head->ptr->next.ptr = item->ptr;
    item->ptr->prev.ptr = head->ptr;
    head->ptr = item->ptr;
}

q_type_ptr q_type_pop(q_type_context * const ctx) {
    q_type_ptr* head = &(ctx->head);
    q_type_ptr tmp;
    if (head->ptr->prev.ptr == 0) {
        return q_type_ptr_null;
    }
    tmp.ptr = head->ptr;
    head->ptr = head->ptr->prev.ptr;
    head->ptr->next.ptr = 0;
    return tmp;
}

void q_type_print(q_type_context * const ctx) {
    q_type_ptr* head = &(ctx->head);
    int i = 0;
    q_type_ptr tmp;
    tmp.ptr = head->ptr;
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

void q_type_free(q_type_context * const ctx, q_type_ptr * const item) {
    q_type_ptr* head = &(ctx->head);
    q_type_ptr tmp;
    tmp.ptr = item->ptr;
    while (ctx->count > 0 && tmp.ptr != 0) {
#ifdef DEBUG
        printf("free: 0x%llx 0x%llx\n", (ADDR)tmp.ptr, (ADDR)tmp.ptr->payload);
#endif
        q_type* ptr = tmp.ptr;
        tmp.ptr = tmp.ptr->next.ptr;
        free(ptr);
        ctx->count--;
    }
#ifdef DEBUG
    printf("\n");
#endif
}

void q_type_alloc(q_type_context * const ctx, abstract_ptr payload) {
    q_type_ptr* head = &(ctx->head);
    q_type_ptr tmp;
    tmp.ptr = (q_type*)malloc(sizeof(q_type));
    tmp.ptr->payload = payload;
#ifdef DEBUG
    printf("alloc: 0x%llx 0x%llx\n", (ADDR)tmp.ptr, (ADDR)tmp.ptr->payload);
#endif
    q_type_push(ctx, &tmp);
    ctx->count++;
}

void q_type_init(q_type_context * const ctx) {
    ctx->head.ptr = (q_type*)malloc(sizeof(q_type));
    ctx->count = 0;
}

void q_type_destroy(q_type_context * const ctx) {
    q_type_ptr* head = &(ctx->head);
    q_type_ptr tmp;
    tmp.ptr = head->ptr;
    while (ctx->count > 0 && tmp.ptr != 0) {
#ifdef DEBUG
        printf("free: 0x%llx 0x%llx\n", (ADDR)tmp.ptr, (ADDR)tmp.ptr->payload);
#endif
        q_type* ptr = tmp.ptr;
        tmp.ptr = tmp.ptr->prev.ptr;
        free(ptr);
        ctx->count--;
    }
#ifdef DEBUG
    printf("\n");
#endif
}

typedef struct list_ptr {
    struct list *ptr;
} list_ptr;

typedef struct list {
    q_type_context context;
    void (*push)(q_type_context * const ctx, q_type_ptr * const item);
    q_type_ptr (*pop)(q_type_context * const ctx);
    void (*print)(q_type_context * const ctx);
    void (*free)(q_type_context * const ctx, q_type_ptr * const item);
    void (*alloc)(q_type_context * const ctx, abstract_ptr payload);
} list;

void list_q_type_push(list_ptr * const head, q_type_ptr* const item) {
    head->ptr->push(&(head->ptr->context), item);
}

q_type_ptr list_q_type_pop(list_ptr * const head) {
    return head->ptr->pop(&(head->ptr->context));
}

void list_q_type_print(list_ptr * const head) {
    head->ptr->print(&(head->ptr->context));
}

void list_q_type_free(list_ptr * const head, q_type_ptr * const item) {
    head->ptr->free(&(head->ptr->context), item);
}

void list_q_type_alloc(list_ptr * const head, abstract_ptr payload) {
    head->ptr->alloc(&(head->ptr->context), payload);
}

void list_init(list_ptr* const head) {
    head->ptr = (list*)malloc(sizeof(list));
    head->ptr->alloc = q_type_alloc;
    head->ptr->push = q_type_push;
    head->ptr->pop = q_type_pop;
    head->ptr->print = q_type_print;
    head->ptr->free = q_type_free;
    q_type_init(&(head->ptr->context));
}

void list_destroy(list_ptr * const head) {
    q_type_destroy(&(head->ptr->context));
    free(head->ptr);
}

void list_using(list_ptr * const head) {
    abstract_ptr payload = (abstract_ptr)0xdeadbeef;
    list_q_type_alloc(head, payload);
    list_q_type_alloc(head, ++payload);
    list_q_type_alloc(head, ++payload);
    list_q_type_alloc(head, ++payload);
    list_q_type_alloc(head, ++payload);
#ifdef DEBUG
    printf("\n");
#endif
#ifdef DEBUG
    list_q_type_print(head);
#endif
    q_type_ptr q_pop0 = list_q_type_pop(head); 
    list_q_type_free(head, &q_pop0);
#ifdef DEBUG
    list_q_type_print(head);
#endif
    q_type_ptr q_pop1 = list_q_type_pop(head); 
    list_q_type_free(head, &q_pop1);
#ifdef DEBUG
    list_q_type_print(head);
#endif
    q_type_ptr q_pop2 = list_q_type_pop(head); 
    list_q_type_free(head, &q_pop2);
#ifdef DEBUG
    list_q_type_print(head);
#endif
    q_type_ptr q_pop3 = list_q_type_pop(head); 
    list_q_type_push(head, &q_pop3);
    q_pop3 = list_q_type_pop(head); 
    list_q_type_free(head, &q_pop3);
#ifdef DEBUG
    list_q_type_print(head);
#endif
    q_type_ptr q_pop4 = list_q_type_pop(head); 
    list_q_type_free(head, &q_pop4);
#ifdef DEBUG
    list_q_type_print(head);
#endif
    q_type_ptr q_pop5 = list_q_type_pop(head); 
    list_q_type_free(head, &q_pop5);
#ifdef DEBUG
    list_q_type_print(head);
#endif
}

void using_list() {
    list_ptr head;
    list_init(&head);
    list_using(&head);
    list_destroy(&head);
}

int main() {
    using_list();
}