/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   April 8, 2025 at 12:32:23 PM GMT+3
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

#ifndef _STD_COMPILE_H_
#define _STD_COMPILE_H_

#define USING_COMPILE

#ifdef CONFIG_TTY
#if CONFIG_TTY == 1
#define USE_TTY
#endif
#endif

#ifdef CONFIG_MEMORY_DEBUG_INFO
#if CONFIG_MEMORY_DEBUG_INFO == 1
#define USE_MEMORY_DEBUG_INFO
#endif
#endif

#ifdef CONFIG_MEMORY_CLEANUP
#if CONFIG_MEMORY_CLEANUP == 1
#define USE_MEMORY_CLEANUP
#endif
#endif

#ifdef CONFIG_MEMORY_ALLOC
#if CONFIG_MEMORY_ALLOC == 1
#define USE_MEMORY_ALLOC
#endif
#endif

#ifdef CONFIG_GC
#if CONFIG_GC == 1
#define USE_GC
#endif
#endif

#ifdef CONFIG_MOCKS
#if CONFIG_MOCKS == 1
#define USE_MOCKS
#endif
#endif

#ifdef CONFIG_MEMCPY
#if CONFIG_MEMCPY == 1
#define USE_MEMCPY
#endif
#endif

#endif /* _STD_COMPILE_H_ */
