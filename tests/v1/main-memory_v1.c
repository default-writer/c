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

extern const union memory_allocator_api memory_allocator_v1;
extern const union memory_allocator_api memory_allocator_v2;
extern const union memory_allocator_api memory_allocator_v3;

void use(const union memory_allocator_api* allocator) {
    /* initializes memory pool */
    allocator->v1.init();
    /* allocation size aligned to 8 byte boundaries (64-bit pointers) */
    u64 size = 2;
    /* allocates memory block */
    void** ptr = allocator->v1.alloc(size);
    for (u64 i = 0; i < size; i++) {
        *(ptr + i) = (void*)0x7bde84217bde8421;
    }
    /* allocation size aligned to 8 byte boundaries (64-bit pointers) */
    u64 size2 = 3;
    /* allocates memory block */
    void** ptr2 = allocator->v1.alloc(size2);
    for (u64 i = 0; i < size2; i++) {
        *(ptr2 + i) = (void*)0x7bde84217bde8421;
    }
    /* allocation size aligned to 8 byte boundaries (64-bit pointers) */
    u64 size3 = 16;
    /* allocates memory block */
    void** ptr3 = allocator->v1.alloc(size3);
    for (u64 i = 0; i < size3; i++) {
        *(ptr3 + i) = (void*)0x7bde84217bde8421;
    }
    /* releases memory block */
    allocator->v1.free(ptr3, size3);
    /* releases memory block */
    allocator->v1.free(ptr2, size2);
    /* releases memory block */
    allocator->v1.free(ptr, size);
    /* destroys memory pool */
    allocator->v1.destroy();
}

int main(void) {
    global_statistics();
    use(&memory_allocator_v1);
    use(&memory_allocator_v2);
    use(&memory_allocator_v3);
    global_statistics();
    return 0;
}
