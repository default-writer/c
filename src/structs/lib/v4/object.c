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

#include "../../common/v4/type.h"
#include "../../common/v4/object.h"

static struct object* object_create(const struct class* class);
static void object_destroy(struct object* ptr);

static struct object* object_create(const struct class* class_ptr) {
    struct typeinfo* ti = class_ptr->type();
    struct methodinfo* mi = class_ptr->method();
    void* data = memory->alloc(sizeof(struct class*) + ti->size);
    struct class** b_class_ptr = (struct class**)data;
    memcpy(b_class_ptr, &class_ptr, sizeof(struct class*));
    void* ptr = (void*)(++b_class_ptr);
    mi->create((struct B*)ptr);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("creating object of class %s at %016llx (%ld bytes)\n", ti->name, (u64)ptr, ti->size);
#endif
    return ptr;
}

static void object_destroy(struct object* ptr) {    
    struct class** b_class_ptr = (struct class**)ptr;
    void** data = (void**)(--b_class_ptr);
    struct class* class_ptr = (struct class*)*data;
    struct typeinfo* ti = class_ptr->type();
    struct methodinfo* mi = class_ptr->method();
    mi->destroy((struct B*)ptr);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("deleting object of class %s at %016llx (%ld bytes)\n", ti->name, (u64)ptr, ti->size);
#endif
    memory->free(data, ti->size + sizeof(struct class*));
}

const struct object_methods object_methods_definition = {
    .create = object_create,
    .destroy = object_destroy
};
