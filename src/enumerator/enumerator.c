#include "common/alloc.h"

#include "list-micro/data.h"

#include "enumerator/enumerator.h"

static void* list_data_enumerator_next(struct enumerator_data* enumerator) {
    struct list_data* list = (struct list_data*)enumerator->current;
    void* data = 0;
    if (list != 0) {
        enumerator->current = (void*)list->next;
        data = list->data;
        enumerator->value = data;
    }
    return data;
}

static struct enumerator_data* list_enumerator_init(void) {
    return _list_alloc(sizeof(struct enumerator_data));
}

static void list_enumerator_destroy(struct enumerator_data* enumerator) {
    _list_free(enumerator, sizeof(struct enumerator_data));
}

const struct list_data_enumerator list_data_enumerator_definition = {
    .init = list_enumerator_init,
    .destroy = list_enumerator_destroy,
    .next = list_data_enumerator_next
};
