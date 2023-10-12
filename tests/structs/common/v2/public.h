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
#ifndef _PUBLIC_V2_H_
#define _PUBLIC_V2_H_

#include "std/common.h"

struct A {
    u64 counter_a;
};

struct B;

struct public_B { /* public */
    struct A base;
    /* public methods */
    void (*set_counter_b)(struct B* ptr, u64 value);
};

struct typeinfo;

struct B_methods {
    void (*set_counter_b)(struct B* ptr, u64 value);
    u64 (*get_counter_b)(const struct B* ptr);
    struct typeinfo* (*type)(void);
};

extern const struct B_methods B_methods_definition;

#if defined(INLINE)
const struct B_methods* B = &B_methods_definition;
#else
static const struct B_methods* B = &B_methods_definition;
#endif

#endif /* _PUBLIC_V2_H_ */
