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
#include "class/class.h"
#include "common/alloc.h"

static object_typeinfo class_create(typeinfo ti);
static void class_destroy(object_typeinfo ptr);

typedef struct writeable_object_typeinfo {
    object object;
    typeinfo typeinfo;
} struct_writeable_object_typeinfo, *writeable_object_typeinfo;

static object_typeinfo class_create(typeinfo ti) {
    object_typeinfo ptr;
#ifndef USE_MEMCPY
    writeable_object_typeinfo type = global_alloc(sizeof(struct_object_typeinfo));
    type->object = global_alloc(ti->size);
    type->typeinfo = ti;
    ptr = (object_typeinfo)type;
#else
    ptr = global_alloc(sizeof(object_typeinfo));
    struct_object_typeinfo object_ti = {
        .object = global_alloc(t->size),
        .typeinfo = ti
    };
    memcpy(ptr, &object_ti, sizeof(object_typeinfo));
#endif
#ifdef USE_MEMORY_DEBUG_INFO
    printf("creating type %s of size %ld (+ %ld)\n", ti->name, ti->size, sizeof(struct_object_typeinfo));
#endif
    return ptr;
}

static void class_destroy(object_typeinfo ptr) {
#ifdef USE_MEMORY_DEBUG_INFO
    printf("deleting type %s of size %ld (+ %ld)\n", ptr->typeinfo->name, ptr->typeinfo->size, sizeof(struct_object_typeinfo));
#endif
    global_free(ptr->object, ptr->typeinfo->size);
    global_free(ptr, sizeof(struct_object_typeinfo));
}

const struct_class class_definition = {
    .create = class_create,
    .destroy = class_destroy
};