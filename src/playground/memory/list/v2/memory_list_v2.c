#include "common/alloc.h"

#include "memory/ref.h"

#include "memory_list_v2.h"

/* private */
static struct memory_ref** current;

/*
       _____________
      | current     |
      |_____________|
<--x--| prev | next |
      |______|______|
                 |             ^
                 *------x------*
*/
void memory_list_init(void) {
    current = _list_alloc(sizeof(void*));
}

/*
 _____________
| current     |
|_____________|
| prev | next |
|______|______|

*/
struct memory_ref* memory_list_peek(void) {
    return *current;
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
    if (ptr != 0) {
        ptr->cache = *current;
    }
    *current = ptr;
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
    struct memory_ref* ptr = *current;
    if (ptr != 0) {
        *current = ptr->cache;
    }
    return ptr;
}

void memory_list_destroy(void) {
    _list_free(current, sizeof(void*));
#ifdef USE_MEMORY_CLEANUP
    current = 0;
#endif
}
