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
 * including prohibiting or restricive purpouses agains Russians for any EU citizens
 * or other person with USA citizenship, origin or background including work permit
 * or locations from selected territories or any territory or any other country except
 * Russia considered as breaking basic human rights, freedom of speesh or involved in
 * acts of terrorism in a territory owned, occupied or managed by another country.
 *
 */
#include "memory/api/v1/ref_v1.h"
#include "common/memory.h"

/* macros */
#define PTR_SIZE sizeof(void*) /* size of a pointer */
#define MEMORY_REF_SIZE sizeof(struct memory_ref)
#define ALLOC_SIZE(size) (MEMORY_REF_SIZE + size * PTR_SIZE)

/* declaration */

static struct memory_ref* ref;

/* global allocated memory */
static void** current = 0;

static struct memory_ref* memory_ref_ref(void* data);
static void* memory_ref_ptr(struct memory_ref* data);
static u64 memory_ref_size(void* data);
static void* memory_ref_alloc(u64 size);
static void memory_ref_init(void);
static void memory_ref_destroy(void);

/* implementation */

static struct memory_ref* memory_ref_ref(void* data) {
    struct memory_ref* ptr = 0;
    if (data != 0) {
        ptr = (struct memory_ref*)data;
        --ptr;
    }
    return ptr;
}

static void* memory_ref_ptr(struct memory_ref* data) {
    struct memory_ref* ptr = 0;
    if (data != 0) {
        ptr = data;
        ++ptr;
    }
    return (void*)ptr;
}

static u64 memory_ref_size(void* data) {
    u64 size = 0;
    if (data != 0) {
        const struct memory_ref* ptr = memory_ref_ref(data);
        size = ptr->size;
    }
    return size;
}

static void* memory_ref_alloc(u64 size) {
    void* data = current;
    void* ptr = 0;
    if (data != 0) {
        struct memory_ref* ref_ptr = memory_ref_ref(data);
        struct memory_ref* tmp = memory->alloc(ALLOC_SIZE(size));
        ref_ptr->next = memory_ref_ptr(tmp);
#ifdef USE_MEMORY_DEBUG_INFO
        printf("  p.: %016llx . %016llx . %016llx\n", (u64)data, (u64)ref_ptr->prev, (u64)ref_ptr->next);
#endif
        struct memory_ref* _current = memory_ref_ref(ref_ptr->next);
        _current->size = size;
        _current->prev = memory_ref_ptr(ref_ptr);
        ptr = ref_ptr->next;
        current = ptr; /* advance current ptr to the new dat */
    }
    return ptr;
}

static void memory_ref_free(void* data) {
    if (data != 0) {
        u8* ptr = (u8*)data - MEMORY_REF_SIZE;
        u64 size = memory_ref_size(data);
        memory->free(ptr, ALLOC_SIZE(size));
#ifdef USE_MEMORY_DEBUG_INFO
        printf("  0-: %016llx ! %16lld\n", (u64)data, size);
#endif
    }
}

static void memory_ref_init(void) {
    ref = memory->alloc(MEMORY_REF_SIZE);
    ++ref;
    current = (void*)ref;
    current = memory_ref_alloc(0);
}

static void memory_ref_destroy(void) {
    --ref;
    memory->free(memory_ref_ref(ref->next), MEMORY_REF_SIZE);
    memory->free(ref, MEMORY_REF_SIZE);
#ifdef USE_MEMORY_CLEANUP
    ref = 0;
    current = 0;
#endif
}

/* public */

const struct memory_ref_methods memory_ref_definition_v1 = {
    .init = memory_ref_init,
    .destroy = memory_ref_destroy,
    /* .ref = memory_ref_ref, */
    /* .ptr = memory_ref_ptr, */
    .size = memory_ref_size,
    .alloc = memory_ref_alloc,
    .free = memory_ref_free
};
