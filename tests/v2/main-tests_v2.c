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
#include "common/alloc.h"

#include "../tests/src/test.h"

extern const struct test_suite vm_v2_test_suite_definition;
static const struct test_suite* vm_v2_tests = &vm_v2_test_suite_definition;

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
    int vm_v2 = vm_v2_tests->run();
#ifndef ATTRIBUTE
    destroy();
#endif
    return vm_v2;
}
