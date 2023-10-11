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
#include "std/common.h"
#include "../common/object.h"

void* object_create(struct typeinfo* ti);
void object_destroy(struct typeinfo* ti);

void* object_create(struct typeinfo* ti) {
    ti->ptr = memory->alloc(ti->size);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("creating type %s (%ld bytes)\n", ti->name, ti->size);
#endif
    return ti->ptr;
}

void object_destroy(struct typeinfo* ti) {
#ifdef USE_MEMORY_DEBUG_INFO
    printf("deleting type %s (%ld bytes)\n", ti->name, ti->size);
#endif
    memory->free(ti->ptr, ti->size);
}

const struct object_methods object_methods_definition = {
    .create = object_create,
    .destroy = object_destroy
};
