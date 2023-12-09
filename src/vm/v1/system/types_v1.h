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
#ifndef _POINTER_TYPES_V1_H_
#define _POINTER_TYPES_V1_H_

#include "vm/v1/types/data/data_v1.h"
#include "vm/v1/types/file/file_v1.h"
#include "vm/v1/types/list/list_v1.h"
#include "vm/v1/types/object/object_v1.h"
#include "vm/v1/types/string/string_v1.h"
#include "vm/v1/types/string_pointer/string_pointer_v1.h"
#include "vm/v1/types/user/user_v1.h"

enum type {
    /* value used for ephemeral type - null */
    TYPE_NULL = 0,
    /* value used for pointer type - ref */
    TYPE_DATA = 1,
    /* value used for string type - string */
    TYPE_STRING = 2,
    /* value used for string ref type - string ref */
    TYPE_STRING_POINTER = 3,
    /* value used for file type - file */
    TYPE_FILE = 4,
    /* value used for list type - list */
    TYPE_LIST = 5,
    /* value used for object type - object */
    TYPE_OBJECT = 6,
    /* value used for user type - user (id: +0, +1, +2, +3, ...) */
    TYPE_USER = 7
};

struct pointer;

struct vm_type {
    void (*free)(struct pointer* ptr);
};

struct types {
    const struct data_methods* data;
    const struct file_methods* file;
    const struct list_methods* list;
    const struct object_methods* object;
    const struct string_methods* string;
    const struct string_pointer_methods* string_pointer;
    const struct user_methods* user;
};

/* definition */
extern const struct types types_definition;

#if !defined(GLOBAL)

/* definition */
#if defined(INLINE)
const const struct types* types = &types_definition;
#else
static const struct types* types = &types_definition;
#endif

#endif /* !GLOBAL */

#endif /* _POINTER_TYPES_V1_H_ */
