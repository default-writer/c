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

#ifndef _TESTS_SRC_LIST_EXPERIMENTAL_H_
#define _TESTS_SRC_LIST_EXPERIMENTAL_H_

#include "common/lcg.h"
#include "common/memory.h"
#include "common/parameters.h"
#include "list-experimental/data.h"

#define list_init list_experimental_init
#define list_destroy list_experimental_destroy

#include "../tests/src/test.h"

#endif /* _TESTS_SRC_LIST_EXPERIMENTAL_H_ */
