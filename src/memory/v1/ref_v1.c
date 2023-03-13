#include "common/alloc.h"

#include "memory/api/v1/ref.h"

#include "common/alloc.h"

/* declaration */

static const u64 memory_offset;

static struct memory_ref* memory;

// global allocated memory
static void** current = 0;

static struct memory_ref* memory_ref_ref(void* data);
static void* memory_ref_ptr(struct memory_ref* data);
static u64 memory_ref_size(void* data);
static void* memory_ref_alloc(u64 size);
static void memory_ref_init(void);
static void memory_ref_destroy(void);

/* implementation */

static struct memory_ref* memory_ref_ref(void* data) {
    struct memory_ref* ptr = 0;
    if (data != 0) {
        ptr = (struct memory_ref*)data;
        --ptr;
    }
    return ptr;
}

static void* memory_ref_ptr(struct memory_ref* data) {
    struct memory_ref* ptr = 0;
    if (data != 0) {
        ptr = data;
        ++ptr;
    }
    return (void*)ptr;
}

static u64 memory_ref_size(void* data) {
    u64 size = 0;
    if (data != 0) {
        struct memory_ref* ptr = memory_ref_ref(data);
        size = ptr->size;
    }
    return size;
}

static void* memory_ref_alloc(u64 size) {
    void* data = current;
    void* ptr = 0;
    if (data != 0) {
        struct memory_ref* ref_ptr = memory_ref_ref(data);
        struct memory_ref* tmp = _list_alloc((size + memory_offset) * sizeof(void*));
        ref_ptr->next = memory_ref_ptr(tmp);
#ifdef USE_MEMORY_DEBUG_INFO
        printf("  p.: 0x%016llx .0x%016llx .0x%016llx\n", (u64)data, (u64)ref_ptr->prev, (u64)ref_ptr->next);
#endif
        struct memory_ref* _current = memory_ref_ref(ref_ptr->next);
        _current->size = size;
        _current->prev = memory_ref_ptr(ref_ptr);
        ptr = ref_ptr->next;
        current = ptr; // advance current ptr to the new dat
    }
    return ptr;
}

static void memory_ref_free(void* data) {
    if (data != 0) {
        void** ptr = data;
        u64 size = memory_ref_size(data);
        _list_free(ptr - memory_offset, (size + memory_offset) * sizeof(void*));
#ifdef USE_MEMORY_DEBUG_INFO
        printf("  0-: 0x%016llx !  %16lld\n", (u64)data, size);
#endif
    }
}

static void memory_ref_init(void) {
    memory = _list_alloc(sizeof(struct memory_ref));
    ++memory;
    current = (void*)memory;
    current = memory_ref_alloc(0);
}

static void memory_ref_destroy(void) {
    --memory;
    _list_free(memory_ref_ref(memory->next), sizeof(struct memory_ref));
    _list_free(memory, sizeof(struct memory_ref));
#ifdef USE_MEMORY_CLEANUP
    memory = 0;
    current = 0;
#endif
}

/* public */

static const u64 memory_offset = sizeof(struct memory_ref) / sizeof(void*);

const struct memory_ref_methods memory_ref_definition = {
    .init = memory_ref_init,
    .destroy = memory_ref_destroy,
    // .ref = memory_ref_ref,
    // .ptr = memory_ref_ptr,
    .size = memory_ref_size,
    .alloc = memory_ref_alloc,
    .free = memory_ref_free
};
