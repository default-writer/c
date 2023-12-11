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

#ifndef _POINTER_TYPES_STRING_H_
#define _POINTER_TYPES_STRING_H_

#include "std/common.h"

/*! @file string_v1.h
 *  @brief C API / string
 */

struct string_methods_v1 {
    void (*free)(u64 ptr);
    u64 (*copy)(u64 ptr);
    void (*strcpy)(u64 dest, u64 src);
    void (*strcat)(u64 dest, u64 src);
    u64 (*strrchr)(u64 src_ptr, u64 match_prt);
    u64 (*strchr)(u64 src_ptr, u64 match_prt);
    u64 (*match)(u64 src_ptr, u64 match_prt);
    u64 (*offset)(u64 src_ptr, u64 match_prt);
    u64 (*load)(const char* data);
    void (*put_char)(u64 ptr, char value);
    char* (*unsafe)(u64 ptr);
    u64 (*size)(u64 ptr);
    u64 (*lessthan)(u64 src, u64 dest);
    u64 (*greaterthan)(u64 src, u64 dest);
    u64 (*equals)(u64 src, u64 dest);
    u64 (*compare)(u64 src, u64 dest);
    u64 (*left)(u64 src, u64 offset);
    u64 (*strncpy)(u64 src, u64 nbytes);
    u64 (*left_strncpy)(u64 src, u64 offset);
    u64 (*right)(u64 src, u64 offset);
    u64 (*move_left)(u64 src, u64 nbytes);
    u64 (*move_right)(u64 src, u64 nbytes);
    u64 (*strcmp)(u64 src, u64 dest);
};

#endif /* _POINTER_TYPES_STRING_H_ */
