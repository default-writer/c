/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 22, 2025 at 11:24:54 AM GMT+3
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

#include "info_v1.h"

/* public */
const system_info_methods PRIVATE_API(system_info_methods_definitions) = {
    .timestamp = UNIX_TIMESTAMP,
    .commit = GIT_COMMIT_HASH,
    .version = API_VERSION
};

const system_info_methods* info = &PRIVATE_API(system_info_methods_definitions);

const system_info_methods* CALL(system_info) {
    return info;
}
