/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   April 7, 2025 at 3:39:43 AM GMT+3
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
PUBLIC const system_error_methods* PUBLIC_API(error) {
    return CALL(error);
}

PUBLIC const system_info_methods* PUBLIC_API(info) {
    return CALL(info);
}

PUBLIC const system_list_methods* PUBLIC_API(list) {
    return CALL(list);
}

PUBLIC const system_memory_methods* PUBLIC_API(memory) {
    return CALL(memory);
}

PUBLIC const system_options_methods* PUBLIC_API(options) {
    return CALL(options);
}

PUBLIC const system_os_methods* PUBLIC_API(os) {
    return CALL(os);
}

PUBLIC const system_hashtable_methods* PUBLIC_API(hashtable) {
    return CALL(hashtable);
}
