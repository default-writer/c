/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 11, 2025 at 9:16:47 PM GMT+3
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* api */
const virtual_api_type PRIVATE_API(virtual_api_methods_definitions) = {
    .fclose = &fclose,
    .fopen = &fopen,
    .fread = &fread,
    .fseek = &fseek,
    .ftell = &ftell,
    .getcwd = &getcwd,
    .getenv = &getenv,
    .memcpy = &memcpy,
    .memset = &memset,
    .puts = &puts,
    .strcmp = &strcmp,
    .strncat = &strncat,
    .strncpy = &strncpy,
    .strlen = &strlen
};

const virtual_api_type* virtual_api = &PRIVATE_API(virtual_api_methods_definitions);
