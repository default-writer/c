/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   February 17, 2025 at 5:07:07 PM GMT+3
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

#ifndef _STD_HEADERS_H_
#define _STD_HEADERS_H_

#define USING_HEADERS

#include <stdint.h>
#include <time.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <time.h>
#include <windows.h>
#define PATH_MAX MAX_PATH
#else
#include <sys/resource.h>
#include <unistd.h>
#if defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 199309L
#include <time.h>
#else
#include <sys/time.h>
#endif
#endif

#include <limits.h>

#endif /* _STD_HEADERS_H_ */
