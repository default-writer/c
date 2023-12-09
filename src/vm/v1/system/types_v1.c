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
#include "types_v1.h"
#include "vm/v1/types/data/data_v1.h"
#include "vm/v1/types/file/file_v1.h"
#include "vm/v1/types/list/list_v1.h"
#include "vm/v1/types/object/object_v1.h"
#include "vm/v1/types/string/string_v1.h"
#include "vm/v1/types/string_pointer/string_pointer_v1.h"
#include "vm/v1/types/user/user_v1.h"

#if !defined(GLOBAL)

/* definition */
#if defined(INLINE)
const struct data_methods* data = &data_methods_definition;
const struct file_methods* file = &file_methods_definition;
const struct list_methods* list = &list_methods_definition;
const struct object_methods* object = &object_methods_definition;
const struct string_methods* string = &string_methods_definition;
const struct string_pointer_methods* string_pointer = &string_pointer_methods_definition;
const struct user_methods* user = &user_methods_definition;
#else
static const struct data_methods* data = &data_methods_definition;
static const struct file_methods* file = &file_methods_definition;
static const struct list_methods* list = &list_methods_definition;
static const struct object_methods* object = &object_methods_definition;
static const struct string_methods* string = &string_methods_definition;
static const struct string_pointer_methods* string_pointer = &string_pointer_methods_definition;
static const struct user_methods* user = &user_methods_definition;
#endif

#endif /* !GLOBAL */

/* public */
const struct types types_definition = {
    .data = &data_methods_definition,
    .list = &list_methods_definition,
    .file = &file_methods_definition,
    .object = &object_methods_definition,
    .string = &string_methods_definition,
    .string_pointer = &string_pointer_methods_definition,
    .user = &user_methods_definition
};
