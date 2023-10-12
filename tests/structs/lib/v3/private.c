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
#include "../../common/v3/type.h"
#include "../../common/v3/public.h"

struct private_B { /* private */
    u64 counter_b;
};

struct B {
    struct public_B public;
    struct private_B private;
};

static void B_create(struct B* ptr);
static void B_destroy(struct B* ptr);

struct typeinfo ti = {
    .size = sizeof(struct B),
#ifdef USE_MEMORY_DEBUG_INFO
    .name = "B"
#endif
};

struct methodinfo mi = {
    .create = B_create,
    .destroy = B_destroy
};

static struct typeinfo* B_typeinfo = &ti;
static struct methodinfo* B_methodinfo = &mi;

static void b_set_counter_b(struct B* ptr, u64 value);
static u64 b_get_counter_b(const struct B* ptr);
static struct typeinfo* b_type(void);
static struct methodinfo* b_methodinfo(void);

static void B_create(struct B* ptr) {
    struct B* b = (struct B*)ptr;
    b->public.get_counter_b = b_get_counter_b;
    b->public.set_counter_b = b_set_counter_b;
}

static void B_destroy(struct B* ptr) {
    struct B* b = (struct B*)ptr;
    b->public.get_counter_b = 0;
    b->public.set_counter_b = 0;
}

static void b_set_counter_b(struct B* ptr, u64 value) {
    ptr->private.counter_b = value;
}

static u64 b_get_counter_b(const struct B* ptr) {
    return ptr->private.counter_b;
}

static struct typeinfo* b_type(void) {
    return B_typeinfo;
}

static struct methodinfo* b_method(void) {
    return B_methodinfo;
}

const struct class B_class_definition = {
    .type = b_type,
    .method = b_method
};
