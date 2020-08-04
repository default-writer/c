#include <stdio.h>
#include <stdlib.h>
#include "list.h"

#define DEBUG

void using_list() {
    list_ptr head;
    list_init(&head);
    list_using(&head);
    list_destroy(&head);
}

int main() {
    using_list();
}