#include "src/list/list.h"

extern const struct test_suite list_test_suite_definition;
static const struct test_suite* list_tests = &list_test_suite_definition;

extern const struct test_suite list_alloc_test_suite_definition;
static const struct test_suite* list_alloc_tests = &list_alloc_test_suite_definition;

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

int main(void) {
#ifndef ATTRIBUTE
    init();
#endif
    int result_alloc = list_alloc_tests->run();
    int result = list_tests->run();
#ifndef ATTRIBUTE
    destroy();
#endif
    return result_alloc | result;
}
