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

typedef struct object* object;
typedef const struct typeinfo* typeinfo;
typedef const struct object_typeinfo* object_typeinfo;

object base_create(const typeinfo ti);
void base_destroy(const object_typeinfo ptr);

struct object { };

struct base {
    object (*create)(const typeinfo ti);
    void (*destroy)(const object_typeinfo ptr);
};

struct typeinfo {
    const size_t size;
#ifdef USE_MEMORY_DEBUG_INFO
    const char* name;
#endif
};

struct object_typeinfo {
    const object object;
#ifdef USE_MEMORY_DEBUG_INFO
    const typeinfo typeinfo;
#else
    u64 size;
#endif
};

const struct base base_methods = {
    .create = base_create,
    .destroy = base_destroy
};

object base_create(const typeinfo t) {
    object obj = (object)memory->alloc(t->size);
#ifdef USE_MEMORY_DEBUG_INFO
    printf("creating type %s of size %ld\n", t->name, t->size);
#endif
    return obj;
}

void base_destroy(const object_typeinfo ptr) {
#ifdef USE_MEMORY_DEBUG_INFO
    printf("deleting type %s of size %ld\n", ptr->typeinfo->name, ptr->typeinfo->size);
    memory->free(ptr->object, ptr->typeinfo->size);
#else
    memory->free(ptr->object, ptr->size);
#endif
}

int main(void) {
#ifdef USE_MEMORY_DEBUG_INFO
#if defined(VM_GLOBAL_DEBUG_INFO)
    global_statistics();
#endif
#endif
    typedef struct B _B;
    typedef _B* B;

    struct A {
        u64 counter_a;
    };

    struct B {
        struct A base;
        u64 counter_b;
    };

    struct typeinfo ti = {
        .size = sizeof(_B),
#ifdef USE_MEMORY_DEBUG_INFO
        .name = "B"
#endif
    };

    const struct base* base = &base_methods;
    const B b = (B)(void*)base->create(&ti);

    b->base.counter_a = 1;
    b->counter_b = 2;

    printf("counter a: 0x%0llx\n", b->base.counter_a);
    printf("counter b: 0x%0llx\n", b->counter_b);

    const struct object_typeinfo obj = {
        .object = (object)b,
#ifdef USE_MEMORY_DEBUG_INFO
        .typeinfo = &ti
#else
        .size = ti.size
#endif
    };

    base->destroy(&obj);
#ifdef USE_MEMORY_DEBUG_INFO
#if defined(VM_GLOBAL_DEBUG_INFO)
    global_statistics();
#endif
#endif
    return 0;
}
