/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 28, 2025 at 4:54:24 PM GMT+3
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

#include "api_v1.h"

/* api */
const virtual_vm_methods* PUBLIC_API(vm) {
    return CALL(vm);
}

const virtual_methods* PUBLIC_API(virtual) {
    return CALL(virtual);
}

const virtual_pointer_methods* PUBLIC_API(pointer) {
    return CALL(pointer);
}

const virtual_env_methods* PUBLIC_API(env) {
    return CALL(env);
}

const virtual_hashtable_methods* PUBLIC_API(hashtable) {
    return CALL(hashtable);
}

const virtual_data_methods* PUBLIC_API(data) {
    return CALL(data);
}

const virtual_file_methods* PUBLIC_API(file) {
    return CALL(file);
}

const virtual_object_methods* PUBLIC_API(object) {
    return CALL(object);
}

const virtual_stack_methods* PUBLIC_API(stack) {
    return CALL(stack);
}

const virtual_string_methods* PUBLIC_API(string) {
    return CALL(string);
}

const virtual_string_pointer_methods* PUBLIC_API(string_pointer) {
    return CALL(string_pointer);
}

const virtual_user_methods* PUBLIC_API(user) {
    return CALL(user);
}
