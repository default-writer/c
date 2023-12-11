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
#ifdef USE_MEMORY_DEBUG_INFO
#if defined(VM_GLOBAL_DEBUG_INFO)
    global_statistics();
#endif
#endif
    use(&memory_allocator_v1);
    use(&memory_allocator_v2);
    use(&memory_allocator_v3);
#ifdef USE_MEMORY_DEBUG_INFO
#if defined(VM_GLOBAL_DEBUG_INFO)
    global_statistics();
#endif
#endif
    return 0;
}
