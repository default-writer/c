#include "common/alloc.h"

#include "enumerator/enumerator.h"

static void* list_data_enumerator_next(struct enumerator_data* enumerator);

struct enumerator_data* list_enumerator_init(void);
void list_enumerator_destroy(struct enumerator_data* enumerator);

struct enumerator_data* list_enumerator_init(void) {
    struct enumerator_data* enumerator = _list_alloc(sizeof(struct enumerator_data));
    return enumerator;
}

void list_enumerator_destroy(struct enumerator_data* enumerator) {
    _list_free(enumerator, sizeof(struct enumerator_data));
}

static void* list_data_enumerator_next(struct enumerator_data* enumerator) {
    void* data = 0;
    data = enumerator->current;
    return data;
}

const struct list_data_enumerator list_data_enumerator_definition = {
    .next = list_data_enumerator_next
};
