#include "common/alloc.h"

#include "list-micro/data.h"

#include "enumerator/enumerator.h"

static void list_enumerator_init(struct enumerator_data** enumerator);
static void list_enumerator_destroy(struct enumerator_data** enumerator);
static void list_data_enumerator_reset(struct enumerator_data** enumerator);
static void* list_data_enumerator_next(struct enumerator_data** enumerator);

static void list_enumerator_init(struct enumerator_data** enumerator) {
    if (enumerator != 0 && *enumerator != 0) {
        *enumerator = _list_alloc(sizeof(struct enumerator_data));
    }
}

static void list_enumerator_destroy(struct enumerator_data** enumerator) {
    if (enumerator != 0 && *enumerator != 0) {
        _list_free(*enumerator, sizeof(struct enumerator_data));
        *enumerator = 0;
    }
}

static void list_data_enumerator_reset(struct enumerator_data** enumerator) {
    if (enumerator != 0 && *enumerator != 0) {
        struct enumerator_data* ptr = *enumerator;
        ptr->current = ptr->initial;
    }
}

static void* list_data_enumerator_next(struct enumerator_data** enumerator) {
    void* data = 0;
    if (enumerator != 0 && *enumerator != 0) {
        struct enumerator_data* ptr = *enumerator;
        struct list_data* list = (struct list_data*)ptr->current;
        if (list != 0) {
            ptr->current = (void*)list->next;
            ptr->value = list->data;
            data = ptr->value;
        }
    }
    return data;
}

const struct list_data_enumerator list_data_enumerator_definition = {
    .init = list_enumerator_init,
    .destroy = list_enumerator_destroy,
    .reset = list_data_enumerator_reset,
    .next = list_data_enumerator_next
};
