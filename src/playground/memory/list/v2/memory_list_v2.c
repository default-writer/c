#include "common/alloc.h"

#include "memory/ref.h"

#include "memory_list_v2.h"

struct memory_ref* current;

/*
       _____________
      | current     |
      |_____________|
<--x--| prev | next |
      |______|______|
                 |             ^
                 *------x------*
*/
void memory_list_init(struct memory_ref* ptr) {
    current = ptr;
    current->prev = 0;
    current->next = 0;
}

/*
 _____________
| current     |
|_____________|
| prev | next |
|______|______|

*/
struct memory_ref* memory_list_peek(void) {
    return current;
}

/*
 _____________        _____________
| ptr         |      | current     |
|_____________|      |_____________|
| prev | next |<-----| prev | next |
|______|______|      |______|______|
    |     |             ^
<---*     *-------------*

*/
void memory_list_push(struct memory_ref* ptr) {
    if (current != 0) {
        current->prev = ptr;
        ptr->next = current;
        ptr->prev = 0;
        current = ptr;
    }
}

/*
 _____________        _____________
| current     |      | ptr         |
|_____________|      |_____________|
| prev | next |<--x--| prev | next |
|______|______|      |______|______|
    |     |             ^
<---*     *------x------*

*/
struct memory_ref* memory_list_pop(void) {
    struct memory_ref* ptr = 0;
    if (current != 0) {
        ptr = current->next;
        current->next = 0;
        current = ptr;
    }
    if (ptr != 0) {
        ptr->prev = 0;
    }
    return ptr;
}

void memory_list_destroy(void) {
#ifdef USE_MEMORY_CLEANUP
    current = 0;
#endif
}
