/*
 *
 * MIT License
 *
 * Copyright (c) 2023 Artur Mustafin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */
#include "vm/v1/vm_v1.h"

#include "common/alloc.h"
#include "list-micro/data.h"
#include "src/test_pointer/test_pointer.h"

#include "../tests/src/test.h"

extern const struct test_suite pointer_test_suite_definition;
static const struct test_suite* pointer_tests = &pointer_test_suite_definition;

extern const struct test_suite list_alloc_test_suite_definition;
static const struct test_suite* list_alloc_tests = &list_alloc_test_suite_definition;

extern const struct test_suite list_micro_test_suite_definition;
static const struct test_suite* list_micro_tests = &list_micro_test_suite_definition;

extern const struct test_suite vm_v1_test_suite_definition;
static const struct test_suite* vm_v1_tests = &vm_v1_test_suite_definition;

int main(void) {
#ifdef USE_MEMORY_DEBUG_INFO
    global_statistics();
#endif
    TEST_RUN(alloc, list_alloc_tests);
    TEST_RUN(micro, list_micro_tests);
    TEST_RUN(vm_v1, vm_v1_tests);
    TEST_RUN(pointer, pointer_tests);
#ifdef USE_MEMORY_DEBUG_INFO
    global_statistics();
#endif
    return alloc | micro | vm_v1 | pointer;
}
