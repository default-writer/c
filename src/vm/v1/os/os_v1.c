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
#include "common/memory.h"
#include "list-micro/data.h"

#include "../pointer/pointer_v1.h"
#include "../virtual/virtual_v1.h"

#include "../types/types_v1.h"

#include "../os/os_v1.h"

#define DEFAULT_SIZE 0x100

/* api */
const struct os_methods os_methods_definition;

/* definition */
static u64 os_getenv(u64 name);
static u64 os_getcwd(void);
static void os_putc(u64 ptr);

/* implementation */
static u64 os_getenv(u64 ptr) {
    if (ptr == 0) {
        return 0;
    }
    const struct pointer* data_ptr = virtual->read_type(ptr, TYPE_STRING);
    if (data_ptr == 0) {
        return 0;
    }
    const char* name_data = pointer->read(data_ptr);
    u64 value = string->load(getenv(name_data));
    return value;
}

static u64 os_getcwd(void) {
    u64 data_ptr = 0;
    char* src = memory->alloc(PATH_MAX + 1);
    data_ptr = string->load(getcwd(src, PATH_MAX));
    memory->free(src, PATH_MAX + 1);
    return data_ptr;
}

static void os_putc(u64 ptr) {
    const char* unsafe_data = string->unsafe(ptr);
    if (unsafe_data == 0) {
        return;
    }
    puts(unsafe_data);
}

/* public */
const struct os_methods os_methods_definition = {
    .getenv = os_getenv,
    .getcwd = os_getcwd,
    .putc = os_putc
};
