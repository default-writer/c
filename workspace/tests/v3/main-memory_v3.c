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

#include "common/memory.h"
#include "playground/memory/api/memory.h"

extern const struct memory_allocator_v2 memory_allocator_v5;

void use(const struct memory_allocator_v2* allocator) {
    /* initializes memory pool */
    allocator->init();
    /* allocation size aligned to 8 byte boundaries (64-bit pointers) */
    u64 size1 = 2;
    /* allocates memory block */
    void** ptr1 = allocator->alloc(size1);
    for (u64 i = 0; i < size1; i++) {
        *(ptr1 + i) = (void*)0x7bde84217bde8421;
    }
    /* allocation size aligned to 8 byte boundaries (64-bit pointers) */
    u64 size2 = 3;
    /* allocates memory block */
    void** ptr2 = allocator->alloc(size2);
    for (u64 i = 0; i < size2; i++) {
        *(ptr2 + i) = (void*)0x7bde84217bde8421;
    }
    /* allocation size aligned to 8 byte boundaries (64-bit pointers) */
    u64 size3 = 16;
    /* allocates memory block */
    void** ptr3 = allocator->alloc(size3);
    for (u64 i = 0; i < size3; i++) {
        *(ptr3 + i) = (void*)0x7bde84217bde8421;
    }
    /* releases memory block */
    allocator->free(ptr2);
    /* releases memory block */
    allocator->free(ptr3);
    /* allocation size aligned to 8 byte boundaries (64-bit pointers) */
    u64 size4 = 16;
    /* allocates memory block */
    void** ptr4 = allocator->alloc(size4);
    for (u64 i = 0; i < size4; i++) {
        *(ptr4 + i) = (void*)0x7bde84217bde8421;
    }
    /* releases memory block */
    allocator->free(ptr4);
    /* releases memory block */
    allocator->free(ptr1);
    /* destroys memory pool */
    allocator->destroy();
}

int main(void) {
#ifdef USE_MEMORY_DEBUG_INFO
#if defined(VM_GLOBAL_DEBUG_INFO)
    global_statistics();
#endif
#endif
    use(&memory_allocator_v5);
#ifdef USE_MEMORY_DEBUG_INFO
#if defined(VM_GLOBAL_DEBUG_INFO)
    global_statistics();
#endif
#endif
    return 0;
}
