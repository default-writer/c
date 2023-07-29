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
#include "common/alloc.h"
#include "playground/memory/api/memory.h"

extern const struct memory_allocator_v2 memory_allocator_v4;

void use(const struct memory_allocator_v2* allocator) {
    /* initializes memory pool */
    allocator->init();
    /* allocation size aligned to 8 byte boundaries (64-bit pointers) */
    u64 size1 = 2;
    /* allocates memory block */
    void** ptr1 = allocator->alloc(size1);
    for (u64 i = 0; i < size1; i++) {
        *(ptr1 + i) = (void*)0xdeadbeefdeadbeef;
    }
    /* allocation size aligned to 8 byte boundaries (64-bit pointers) */
    u64 size2 = 3;
    /* allocates memory block */
    void** ptr2 = allocator->alloc(size2);
    for (u64 i = 0; i < size2; i++) {
        *(ptr2 + i) = (void*)0xdeadbeefdeadbeef;
    }
    /* allocation size aligned to 8 byte boundaries (64-bit pointers) */
    u64 size3 = 16;
    /* allocates memory block */
    void** ptr3 = allocator->alloc(size3);
    for (u64 i = 0; i < size3; i++) {
        *(ptr3 + i) = (void*)0xdeadbeefdeadbeef;
    }
    /* releases memory block */
    allocator->free(ptr3);
    /* allocation size aligned to 8 byte boundaries (64-bit pointers) */
    u64 size4 = 16;
    /* allocates memory block */
    void** ptr4 = allocator->alloc(size4);
    for (u64 i = 0; i < size4; i++) {
        *(ptr4 + i) = (void*)0xdeadbeefdeadbeef;
    }
    /* releases memory block */
    allocator->free(ptr4);
    /* releases memory block */
    allocator->free(ptr2);
    /* releases memory block */
    allocator->free(ptr1);
    /* destroys memory pool */
    allocator->destroy();
}

static void INIT init() {
#ifdef USE_MEMORY_DEBUG_INFO
    global_statistics();
#endif
}

static void DESTROY destroy() {
#ifdef USE_MEMORY_DEBUG_INFO
    global_statistics();
#endif
}

int main(void) {
#ifndef ATTRIBUTE
    init();
#endif
    use(&memory_allocator_v4);
#ifndef ATTRIBUTE
    destroy();
#endif
    return 0;
}
