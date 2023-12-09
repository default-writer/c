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
#ifndef _POINTER_TYPES_STRING_H_
#define _POINTER_TYPES_STRING_H_

#include "std/common.h"

/*! @file string_v1.h
 *  @brief C API / string
 */

struct string_methods {
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

/* definition */
extern const struct string_methods string_methods_definition;

/* definition */
#if defined(INLINE)
const struct string_methods* string = &string_methods_definition;
#else
static const struct string_methods* string = &string_methods_definition;
#endif

#endif /* _POINTER_TYPES_STRING_H_ */
