#include "stl/linked_list.h"
#include "stdlib.h"

/* private */
static struct linked_list* new(void);
static struct linked_list* reverse_list(struct linked_list* head);
static void delete(struct linked_list* head);

/* implementation */
static struct linked_list* reverse_list(struct linked_list* head) {
    struct linked_list* current = head;
    struct linked_list* prev = 0;
    struct linked_list* tmp = 0;
    while (current) {
        tmp = prev;
        prev = current;
        current = current->next;
        prev->next = tmp;
    }
    return prev;
}

static struct linked_list* new(void) {
    struct linked_list* node = calloc(1, sizeof(struct linked_list));
    return node;
}

static void delete(struct linked_list* head) {
    /* gets pointer */
    struct linked_list* ptr = head;
    /* destroys the list */
    while (ptr != 0) {
        struct linked_list* next = ptr->next;
        free(ptr);
        ptr = next;
    }
}

/* public */

const struct linked_list_methods linked_list_methods_definition = {
    /* generic methods */
    .new = new,
    .delete = delete,
    .reverse_list = reverse_list,
};
