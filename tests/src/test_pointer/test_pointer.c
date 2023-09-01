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
#include "test_pointer.h"

/* definition */
extern const struct pointer_methods pointer_methods_definition;
extern const struct list_methods list_methods_definition;
extern const struct file_methods file_methods_definition;
extern const struct string_methods string_methods_definition;
extern const struct user_methods user_methods_definition;
extern const struct data_methods data_methods_definition;
extern const struct object_methods object_methods_definition;
extern const struct os_methods os_methods_definition;
extern const struct string_pointer_methods string_pointer_methods_definition;
extern const struct memory memory_definition;

/* definition */
static const struct pointer_methods* pointer = &pointer_methods_definition;
static const struct list_methods* list = &list_methods_definition;
static const struct file_methods* file = &file_methods_definition;
static const struct string_methods* string = &string_methods_definition;
static const struct user_methods* user = &user_methods_definition;
static const struct data_methods* data = &data_methods_definition;
static const struct object_methods* object = &object_methods_definition;
static const struct os_methods* os = &os_methods_definition;
static const struct string_pointer_methods* string_pointer = &string_pointer_methods_definition;
static const struct memory* memory = &memory_definition;

/* Data structure to use at the core of our fixture. */
typedef struct test_data {
    struct pointer* ptr;
} * TEST_DATA;

/* Initialize the data structure. Its allocation is handled by Rexo. */
RX_SET_UP(test_set_up) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct pointer** ctx = &rx->ptr;
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct pointer** ctx = &rx->ptr;
    *ctx = 0;
}

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

/* test init */
RX_TEST_CASE(pointer_tests, test_pointer_init, .fixture = test_fixture) {
    pointer->init(8);
    pointer->release();
    pointer->destroy();
}

static int run(void) {
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- rexo unit test code\n");
#endif
    /* Execute the main function that runs the test cases found. */
    int result = rx_run(0, NULL) == RX_SUCCESS ? 0 : 1;
    return result;
}

const struct test_suite pointer_test_suite_definition = {
    .run = run
};
