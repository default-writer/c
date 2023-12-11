/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   11 December 2023 at 9:15:15 GMT+3
 *
 */
/*
    Copyright (C) 2022-2047 Artur Mustafin (artur.mustafin@gmail.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
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
