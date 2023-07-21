#include "common/alloc.h"

#include "../tests/src/test.h"

extern const struct test_suite vm_v1_test_suite_definition;
static const struct test_suite* vm_v1_tests = &vm_v1_test_suite_definition;

#define DEFAULT_SIZE 0x100

/* definition */
extern const struct vm_methods vm_methods_definition;

typedef struct test_data {
}* TEST_DATA;

static void INIT init() {
#ifdef USE_MEMORY_DEBUG_INFO
    global_statistics();
#endif
}

static void DESTROY destroy() {
#ifdef USE_MEMORY_DEBUG_INFO
    global_statistics();
#endif
}

int main(int argc, char** argv) {
#ifndef ATTRIBUTE
    init();
#endif
    int vm_v1 = vm_v1_tests->run();
#ifndef ATTRIBUTE
    destroy();
#endif
    return vm_v1;
}
