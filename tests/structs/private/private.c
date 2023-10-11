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
#include "../common/v2/object.h"

#include "public.h"

struct private_B { /* private */
    u64 counter_b;
};

struct friend_B {
    struct public_B public;
    struct private_B private;
};

struct private_type {
    struct typeinfo* info;
};

struct typeinfo ti = {
    .size = sizeof(struct friend_B),
#ifdef USE_MEMORY_DEBUG_INFO
    .name = "B"
#endif
};

static struct typeinfo* B_typeinfo = &ti;

static void b_methods_set_counter_b(struct friend_B* ptr, u64 value);
static u64 b_methods_get_counter_b(struct friend_B* ptr);
static struct typeinfo* b_methods_type(void);

static void b_methods_set_counter_b(struct friend_B* ptr, u64 value) {
    ptr->private.counter_b = value;
}

static u64 b_methods_get_counter_b(struct friend_B* ptr) {
    return ptr->private.counter_b;
}

static struct typeinfo* b_methods_type(void) {
    return B_typeinfo;
}

const struct B_methods B_methods_definition = {
    .set_counter_b = b_methods_set_counter_b,
    .get_counter_b = b_methods_get_counter_b,
    .type = b_methods_type
};
