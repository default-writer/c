#include "public.h"
#include "../common/object.h"

struct private_B { /* private */
    u64 counter_b;
};

struct friend_B {
    struct public_B public;
    struct private_B private;
};

struct typeinfo ti = {
    .size = sizeof(struct friend_B),
#ifdef USE_MEMORY_DEBUG_INFO
    .name = "B"
#endif
};

struct typeinfo ti_definition = {
    .size = sizeof(struct friend_B), //sizeof(struct public_B) + sizeof(struct private_B),
#ifdef USE_MEMORY_DEBUG_INFO
    .name = "B"
#endif
};

static void b_methods_set_counter_b(struct friend_B* ptr, u64 value);
static u64 b_methods_get_counter_b(struct friend_B* ptr);

static void b_methods_set_counter_b(struct friend_B* ptr, u64 value) {
    ptr->private.counter_b = value;
}

static u64 b_methods_get_counter_b(struct friend_B* ptr) {
    return ptr->private.counter_b;
}

const struct B_methods B_methods_definition = {
    .set_counter_b = b_methods_set_counter_b,
    .get_counter_b = b_methods_get_counter_b
};
