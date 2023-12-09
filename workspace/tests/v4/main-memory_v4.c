/*
 *
 * Russian's IP Protection License
 *
 * Copyright (c) 2023 Artur Mustafin
 *
 * Permission is hereby granted, free of charge, to any person with citizenship
 * and location in Russia including Crimea and all occupations obtaining a copy
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
 * For the rest of the world it is an order to pay royalties by agreement to the
 * author of the code base for ability to use any part of the project for any
 * purpouse including but not limited to the creative ideas or technologies are
 * being used in this owned intellectual property.
 *
 * It is strictly prohibited to use this code base or any part of it for any purpouse
 * including prohibiting or restricive purpouses against Russians for any EU citizens
 * or other person with USA citizenship, origin or background including work permit
 * or locations from selected territories or any territory or any other country except
 * Russia considered as breaking basic human rights, freedom of speesh or involved in
 * acts of terrorism in a territory owned, occupied or managed by another country.
 *
 */
#include "common/lcg.h"
#include "common/memory.h"
#include "playground/memory/api/memory.h"

extern const struct memory_allocator_v3 memory_allocator_v6;

void use(const struct memory_allocator_v3* allocator) {
    u64 index = 21;
    /* initializes memory pool */
    allocator->init(index);
    /* allocation size aligned to 8 byte boundaries (64-bit pointers) */
    u64 _size1 = 2;
    /* allocates memory block */
    void** _ptr1 = allocator->alloc(_size1);
    for (u64 i = 0; i < _size1; i++) {
        /* generates random values */
        *(_ptr1 + i) = (void*)lcg_parkmiller_64();
    }
    /* releases memory block */
    allocator->free(_ptr1);
    /* allocation size aligned to 8 byte boundaries (64-bit pointers) */
    u64 _size2 = 3;
    /* allocates memory block */
    void** _ptr2 = allocator->alloc(_size2);
    for (u64 i = 0; i < _size2; i++) {
        *(_ptr2 + i) = (void*)lcg_parkmiller_64();
    }
    /* releases memory block */
    allocator->free(_ptr2);
    /* allocation size aligned to 8 byte boundaries (64-bit pointers) */
    u64 _size3 = 16;
    /* allocates memory block */
    void** _ptr3 = allocator->alloc(_size3);
    for (u64 i = 0; i < _size3; i++) {
        *(_ptr3 + i) = (void*)lcg_parkmiller_64();
    }
    /* releases memory block */
    allocator->free(_ptr3);
    /* allocation size aligned to 8 byte boundaries (64-bit pointers) */
    u64 _size4 = 21;
    /* allocates memory block */
    void** _ptr4 = allocator->alloc(_size4);
    for (u64 i = 0; i < _size4; i++) {
        *(_ptr4 + i) = (void*)lcg_parkmiller_64();
    }
    /* releases memory block */
    allocator->free(_ptr4);
    /* allocation size aligned to 8 byte boundaries (64-bit pointers) */
    u64 _size5 = 22;
    /* allocates memory block */
    void** _ptr5 = allocator->alloc(_size5);
    for (u64 i = 0; i < _size5; i++) {
        *(_ptr5 + i) = (void*)lcg_parkmiller_64();
    }
    /* releases memory block */
    allocator->free(_ptr5);
    /* allocation size aligned to 8 byte boundaries (64-bit pointers) */
    u64 _size6 = 21;
    /* allocates memory block */
    void** _ptr6 = allocator->alloc(_size6);
    for (u64 i = 0; i < _size6; i++) {
        *(_ptr6 + i) = (void*)lcg_parkmiller_64();
    }
    /* releases memory block */
    allocator->free(_ptr6);
    /* allocation size aligned to 8 byte boundaries (64-bit pointers) */
    u64 _size7 = 20;
    /* allocates memory block */
    void** _ptr7 = allocator->alloc(_size7);
    for (u64 i = 0; i < _size7; i++) {
        *(_ptr7 + i) = (void*)lcg_parkmiller_64();
    }
    /* releases memory block */
    allocator->free(_ptr7);
    /* destroys memory pool */
    allocator->destroy();
}

int main(void) {
#ifdef USE_MEMORY_DEBUG_INFO
#if defined(VM_GLOBAL_DEBUG_INFO)
    global_statistics();
#endif
#endif
    use(&memory_allocator_v6);
#ifdef USE_MEMORY_DEBUG_INFO
#if defined(VM_GLOBAL_DEBUG_INFO)
    global_statistics();
#endif
#endif
    return 0;
}
