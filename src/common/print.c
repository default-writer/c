#include "std/api.h"
#include "std/types.h"
#include "common/print.h"

// print head on current context (stack)
void list_print_head(struct list_data** const current, void* (*_data)(struct list_data*)) {
    // get current context's head
    struct list_data* tmp = *current;
    // visualise item
    printf("*: 0x%llx 0x%llx\n", (LPTR)tmp, (LPTR)_data(tmp));
}

// print all stack trace to output
// in a single loop, print out all elements except root element (which does not have a payload)
// as a result, all stack will be printed in last-to-first order (reverse)
void list_print(struct list_data** const current, struct list_data* (*list_next)(struct list_data*), void* (*list_data)(struct list_data*)) {
    // get current context's head
    struct list_data* head = *current;
    // sets the counter
    int i = 0; 
    // assigns current's head pointer to the temporary
    struct list_data* tmp = head;
    if (tmp != 0)
    {
        // until we found root element (element with no previous element reference)
        do {
            // debug output of memory dump
            printf("%d: 0x%llx 0x%llx\n", ++i, (LPTR)tmp, (LPTR)list_data(tmp));
            // remember temprary's prior pointer value to temporary
            tmp = list_next(tmp);
        } while (tmp != 0/*root*/);
    }
    // stop on root element
}