#include "common/alloc.h"

#include "memory/api/ref.h"

#include "memory/api/v2/ref.h"

#include "common/alloc.h"

#include "playground/memory/list/v2/memory_list_v2.h"

#include "std/macros.h"

/* declaration */

static const u64 memory_offset;

static struct memory_ref* memory;
static struct memory_ref* memory_alloc_ptr;
u64 memory_alloc_ptr_size = 0;

// global allocated memory
static void** current = 0;

static struct memory_ref* memory_ref_ref(void* data);
static void* memory_ref_ptr(struct memory_ref* data);
static u64 memory_ref_size(void* data);
static void* memory_ref_alloc(u64 size);
static void memory_ref_free(void* data);
static void memory_ref_init(u64 size);
static void memory_ref_destroy(void);
static void memory_ref_push(void* data);
static void* memory_ref_pop(void);
static void* memory_ref_peek(void);
static void memory_ref_use(void* data);

/* implementation */

static struct memory_ref*
memory_ref_ref(void* data) {
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
    struct memory_ref* data = (struct memory_ref*)current;
    struct memory_ref* ptr = 0;
    if (data != 0) {
        if (size > 0 && memory_alloc_ptr != 0 && memory_alloc_ptr_size >= size + memory_offset) {
            memory_alloc_ptr_size -= size + memory_offset;
            ptr = ((void*)((u64*)memory_alloc_ptr + memory_alloc_ptr_size + memory_offset));
            ptr->prev = 0;
            ptr->next = 0;
            ptr->cache = 0;
            ptr->size = 0;
        } else {
            memory_alloc_ptr = 0;
            memory_alloc_ptr_size = 0;
            void* tmp = memory_ref_peek();
            u64 ptr_size = memory_ref_size(tmp);
            if (ptr_size != 0 && ptr_size >= size) {
                ptr = memory_ref_pop();
            } else {
                // ptr = memory_ref_pop();
                // void* prev = ptr->prev;
                // void* next = ptr->next;
                // CLEAN(prev)
                // CLEAN(next)
                // memory_ref_free(ptr);
                ptr = _list_alloc((size + memory_offset) * sizeof(void*));
                struct memory_ref* ref_ptr = memory_ref_ref(current);
                ref_ptr->next = memory_ref_ptr(ptr);
#ifdef USE_MEMORY_DEBUG_INFO
                printf("  p.: 0x%016llx .0x%016llx .0x%016llx\n", (u64)data, (u64)ref_ptr->prev, (u64)ref_ptr->next);
#endif
                struct memory_ref* _current = memory_ref_ref(ref_ptr->next);
                _current->size = size;
                _current->prev = memory_ref_ptr(ref_ptr);
                ptr = ref_ptr->next;
                current = (void*)ptr; // advance current ptr to the new data
            }
            memory_ref_use(ptr);
        }
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

static void memory_ref_init(u64 size) {
    memory_list_init();
    memory = _list_alloc(sizeof(struct memory_ref));
    ++memory;
    current = (void*)memory;
    current = memory_ref_alloc(0);
    void* data = memory_ref_alloc(size);
    memory_ref_use(data);
}

static void memory_ref_destroy(void) {
    struct memory_ref* data = 0;
    while ((data = memory_ref_pop()) != 0) {
        memory_ref_free(data);
    }
    // if (memory_alloc_ptr != 0) {
    //     void* data = memory_ref_ptr(memory_alloc_ptr);
    //     memory_ref_free(data);
    // }
    --memory;
    _list_free(memory_ref_ref(memory->next), sizeof(struct memory_ref));
    _list_free(memory, sizeof(struct memory_ref));
#ifdef USE_MEMORY_CLEANUP
    memory = 0;
    current = 0;
    memory_alloc_ptr = 0;
    memory_alloc_ptr_size = 0;
#endif
    memory_list_destroy();
}

static void memory_ref_push(void* data) {
    if (data != 0) {
        struct memory_ref* ptr = memory_ref_ref(data);
        if (ptr == memory_alloc_ptr) {
            memory_list_push(ptr);
        }
    }
}

static void* memory_ref_pop(void) {
    void* ptr = 0;
    struct memory_ref* data = memory_list_pop();
    if (data != 0) {
        ptr = memory_ref_ptr(data);
    }
    return ptr;
}

static void* memory_ref_peek(void) {
    void* data = memory_list_peek();
    void* ptr = 0;
    if (data != 0) {
        ptr = memory_ref_ptr(data);
    }
    return ptr;
}

/* public */

static const u64 memory_offset = sizeof(struct memory_ref) / sizeof(void*);

static void memory_ref_use(void* data) {
    u64 size = memory_ref_size(data);
    if (data != 0 && size >= memory_offset) {
        memory_alloc_ptr = memory_ref_ref(data);
        memory_alloc_ptr_size = size;
    }
}

const struct memory_ref_methods memory_ref_definition = {
    .init = memory_ref_init,
    .destroy = memory_ref_destroy,
    // .ref = memory_ref_ref,
    // .ptr = memory_ref_ptr,
    .size = memory_ref_size,
    .alloc = memory_ref_alloc,
    .free = memory_ref_free,
    .push = memory_ref_push,
    .pop = memory_ref_pop,
    .peek = memory_ref_peek
    // .use = memory_ref_use
};
