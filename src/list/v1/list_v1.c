/*
 *
 * MIT License
 *
 * Copyright (c) 2023 Artur Mustafin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */
#include "list/v1/list_v1.h"
#include "common/alloc.h"

/* implementation */

/* definition */
extern const struct memory memory_definition;
static const struct memory* memory = &memory_definition;

/* private */

/* size of a memory block to allocate */
static const size_t _size = sizeof(struct list_v1);

/* allocates memory pointer */
static struct list_v1* _new(void) {
    /* returns list object */
    return memory->alloc(_size);
}

/* releases memory pointer */
static void _delete(struct list_v1* ptr) {
    /* releases the pointer */
    memory->free(ptr, _size);
}

/* gets chunk's next item. external code ensures ptr is not 0 */
static struct list_v1* list_next(struct list_v1* ptr) {
    /* external code ensures prt is not 0 */
    return ptr->next;
}

/* deletes the data pointer */
static void list_delete(struct list_v1* ptr) {
    /* releases the pointer */
    _delete(ptr);
}

/* allocates memory pointer */
static struct list_v1* list_alloc(void* payload) {
    /* creates empty data chunk */
    struct list_v1* item = _new();
    /* writes data into allocated memory buffer */
    item->data = payload;
    /* returns created data structure */
    return item;
}

/* releases memory pointer */
static void list_free(struct list_v1** current) {
    /* gets the current memory pointer */
    struct list_v1* tmp = *current;
    /* checks if pointer is not null */
    if (tmp != 0) {
        /* releases memory */
        list_delete(tmp);
        /* resets current pointer to 0 */
        *current = 0;
    }
}

/* pushes new item to existing context */
static struct list_v1* list_push(struct list_v1** current, struct list_v1* item) {
    const struct list_v1* tmp = *current;
    /* checks if pointer is not null */
    if (tmp != 0 && item != 0) {
        /* gets the current memory pointer */
        struct list_v1* head = *current;
        /* assigns item pointer to head's prev pointer value */
        head->prev = item;
        /* assigns item's next pointer to current pointer */
        item->next = *current;
        /* advances position of head pointer to the new head */
        *current = item;
        /* return previous context's head */
        return head;
    }
    /* if we call method on empty stack, do not return head element, return null element by convention */
    return 0;
}

/* pops existing element at the top of the stack/queue/list */
static struct list_v1* list_pop(struct list_v1** current) {
    const struct list_v1* tmp = *current;
    /* checks if pointer is not null */
    if (tmp != 0) {
        /* gets the current memory pointer */
        struct list_v1* ptr = *current;
        /* gets next pointer */
        struct list_v1* next = list_next(ptr);
        /* root elements returns null, i.e. 0 by convention */
        if (next == 0) {
            /* returns default element as null element */
            return 0;
        }
        /* resets prev pointer */
        next->prev = 0;
#ifdef USE_MEMORY_CLEANUP
        /* points to next node */
        ptr->next = 0;
        /* points to next node */
        ptr->prev = 0;
#endif
        /* rewinds head pointer to next pointer value */
        *current = next;
        /* returns removed element */
        return ptr;
    }
    /* if we call method on empty stack, do not return head element, return null element by convention */
    return 0;
}

/* peeks existing element at the top of the stack/queue/list */
static struct list_v1* list_peek(struct list_v1** current) {
    const struct list_v1* tmp = *current;
    /* checks if pointer is not null */
    if (tmp != 0) {
        /* gets the current memory pointer */
        struct list_v1* ptr = *current;
        /* gets next pointer */
        const struct list_v1* next = list_next(ptr);
        /* root elements returns null, i.e. 0 by convention */
        if (next == 0) {
            /* returns default element as null element */
            return 0;
        }
        /* returns current pointer */
        return ptr;
    }
    /* if we call method on empty stack, do not return head element, return null element by convention */
    return 0;
}

/* initializes the new context's head element */
static void list_init(struct list_v1** current) {
    const struct list_v1* tmp = *current;
    /* checks if pointer is null */
    if (tmp == 0) {
        /* sets the current memory pointer */
        *current = _new();
    }
}

/* destroys the memory stack */
static void list_destroy(struct list_v1** current) {
    /* gets the current memory pointer */
    struct list_v1* tmp = *current;
    /* checks if pointer is not null */
    if (tmp != 0) {
        /* until we found element with no next node (not a list element) */
        do {
            /* gets temporary pointer value */
            struct list_v1* ptr = tmp;
            /* gets prev pointer value */
            struct list_v1* next = list_next(ptr);
            /* releases memory, should check for 0 before execution */
            list_delete(ptr);
            /* advances temporary pointer value to the next item */
            tmp = next;
        } while (tmp != 0);
        /* resets current pointer to 0 */
        *current = 0;
    }
}

#ifdef VM_DEBUG_INFO

/* prints head on current context (stack) */
static void list_print_head(struct list_v1** current) {
    /* get current context's head */
    struct list_v1* ptr = *current;
    /* visualize item */
    printf("   *: %016llx > %016llx\n", (u64)ptr, (u64)list_data_v1(ptr));
}

/* prints all stack trace to output */
static void list_print(struct list_v1** current) {
    /* sets the counter */
    int i = 0;
    /* assigns current's head pointer to the temporary */
    struct list_v1* tmp = *current;
    if (tmp != 0) {
        /* until we found root element (element with no previous element reference) */
        do {
            /* debug output of memory dump */
            printf("%4d: %016llx * %016llx\n", ++i, (u64)tmp, (u64)list_data_v1(tmp));
            /* remember temporary's prior pointer value to temporary */
            tmp = list_next(tmp);
        } while (tmp != 0 /*root*/);
    }
    /* stop on root element */
}

#endif

/* public */

const struct list_methods_v1 list_definition = {
    /* generic methods */
    .init = list_init,
    .destroy = list_destroy,
    /* list methods */
    .alloc = list_alloc,
    .free = list_free,
    .push = list_push,
    .pop = list_pop,
    .peek = list_peek,
#ifdef VM_DEBUG_INFO
    .print_head = list_print_head,
    .print = list_print
#endif
};