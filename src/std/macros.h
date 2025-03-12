/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 12, 2025 at 6:19:13 PM GMT+3
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

#ifndef _STD_MACROS_H_
#define _STD_MACROS_H_

#define USING_MACROS

#define TRUE 1
#define FALSE 0
#define NULL_PTR (void*)0

#define VM_EXPAND(X) X
#define VM_CONCAT_(A, B) A##B
#define VM_CONCAT(A, B) VM_CONCAT_(A, B)
#define VM_STRINGIFY_(X) #X
#define VM_STRINGIFY(X) VM_STRINGIFY_(X)

#define PRIVATE_API(x) VM_CONCAT(API(x), VM_CONCAT(_, GIT_COMMIT_HASH_VALUE))
#define API_VERSION VM_EXPAND(VM_CONCAT(, VM_STRINGIFY(VM_CONCAT(MAJOR_VERSION, VM_CONCAT(., VM_CONCAT(MINOR_VERSION, VM_CONCAT(., PATCH_VERSION)))))))
#define API(x) VM_CONCAT(x, VM_CONCAT(_, VM_CONCAT(v, VM_CONCAT(MAJOR_VERSION, VM_CONCAT(_, VM_CONCAT(MINOR_VERSION, VM_CONCAT(_, PATCH_VERSION)))))))

#define CALL(x) PRIVATE_API(x)()

#ifdef USE_MEMORY_DEBUG_INFO
#define ERROR(message_id, func, args) error_api->stderr(ID_##message_id, func, args, __FILE__, __LINE__)
#define ERROR_NO_ERROR(...) ERROR(ERROR_NO_ERROR, __func__, VM_STRINGIFY(__VA_ARGS__))
#define ERROR_VM_NOT_INITIALIZED(...) ERROR(ERROR_VM_NOT_INITIALIZED, __func__, VM_STRINGIFY(__VA_ARGS__))
#define ERROR_POINTER_NOT_INITIALIZED(...) ERROR(ERROR_POINTER_NOT_INITIALIZED, __func__, VM_STRINGIFY(__VA_ARGS__))
#define ERROR_ADDRESS_NOT_INITIALIZED(...) ERROR(ERROR_ADDRESS_NOT_INITIALIZED, __func__, VM_STRINGIFY(__VA_ARGS__))
#define ERROR_ARGUMENT_VALUE_NOT_INITIALIZED(...) ERROR(ERROR_ARGUMENT_VALUE_NOT_INITIALIZED, __func__, VM_STRINGIFY(__VA_ARGS__))
#define ERROR_INVALID_CONDITION(...) ERROR(ERROR_INVALID_CONDITION, __func__, VM_STRINGIFY(__VA_ARGS__))
#else
#define ERROR(message_id) error_api->stderr(ID_##message_id)
#define ERROR_NO_ERROR(...) ERROR(ERROR_NO_ERROR)
#define ERROR_VM_NOT_INITIALIZED(...) ERROR(ERROR_VM_NOT_INITIALIZED)
#define ERROR_POINTER_NOT_INITIALIZED(...) ERROR(ERROR_POINTER_NOT_INITIALIZED)
#define ERROR_ADDRESS_NOT_INITIALIZED(...) ERROR(ERROR_ADDRESS_NOT_INITIALIZED)
#define ERROR_ARGUMENT_VALUE_NOT_INITIALIZED(...) ERROR(ERROR_ARGUMENT_VALUE_NOT_INITIALIZED)
#define ERROR_INVALID_CONDITION(...) ERROR(ERROR_INVALID_CONDITION)
#endif

#ifdef __GNUC__
#define FALL_THROUGH __attribute__((fallthrough))
#define ATTRIBUTE
#define INIT __attribute__((constructor))
#define DESTROY __attribute__((destructor))
#else
#define INIT
#define DESTROY
#define FALL_THROUGH ((void)0)
#endif /* __GNUC__ >= 7 */

#endif /* _STD_MACROS_H_ */
