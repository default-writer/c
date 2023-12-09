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
#ifndef _VIRTUAL_H_
#define _VIRTUAL_H_

#include "std/common.h"

struct vm;
struct pointer;

struct vm_default_options {
    const u64 size;
};

struct virtual_methods {
    void (*init)(struct vm**, u64 size);
    void (*destroy)(struct vm**);
    u64 (*alloc)(struct pointer* ptr);
    void (*free)(struct pointer* ptr);
    struct pointer* (*read)(u64 address);
    struct pointer* (*read_type)(u64 address, u64 id);
    void (*enumerator_init)(void);
    void (*enumerator_destroy)(void);
    u64 (*enumerator_next)(void);
#if defined(VM_MEMORY_DEBUG_INFO)
    void (*dump)(void);
    void (*dump_ref)(void);
#endif
};

/* definition */
extern const struct virtual_methods virtual_methods_definition;
extern const struct vm_default_options vm_default_options_definition;

/* definition */

#if defined(INLINE)
const struct virtual_methods* virtual = &virtual_methods_definition;
const struct vm_default_options* vm_options = &vm_default_options_definition;
#else
/* definition */
static const struct virtual_methods* virtual = &virtual_methods_definition;
static const struct vm_default_options* vm_options = &vm_default_options_definition;
#endif

#endif /* _VIRTUAL_H_ */
