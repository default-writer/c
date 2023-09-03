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
#include "pointer/types/string_pointer/v1/string_pointer_v1.h"
#include "common/alloc.h"
#include "list-micro/data.h"
#include "pointer/v1/pointer_v1.h"
#include "vm/v1/vm_v1.h"

#define DEFAULT_SIZE 0x100

static const enum type id = TYPE_STRING_POINTER;

/* api */
const struct string_pointer_methods string_pointer_methods_definition;
void string_pointer_init();

extern u64 pointer_vm_register_type(u64 id, const struct vm_type* type);

/* definition */
extern const struct vm_methods vm_methods_definition;
extern const struct pointer_vm_methods pointer_vm_methods_definition;

/* definition */
static const struct vm_methods* vm = &vm_methods_definition;
static const struct pointer_vm_methods* virtual = &pointer_vm_methods_definition;

/* definition */
static const struct vm_type type_definition;
static const struct vm_type* type = &type_definition;

/* internal */
static void string_pointer_vm_free(struct pointer* ptr);

/* implementation */
static void string_pointer_vm_free(struct pointer* ptr) {
    virtual->free(ptr);
}

static const struct vm_type type_definition = {
    .free = string_pointer_vm_free
};

static void INIT init() {
    pointer_vm_register_type(id, type);
}

/* public */
const struct string_pointer_methods string_pointer_methods_definition = {};

#ifndef ATTRIBUTE
void string_pointer_init() {
    init();
}
#endif
