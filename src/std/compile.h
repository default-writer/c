/*
 *
 * MIT License
 *
 * Copyright (c) 2023 Artur Mustafin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
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
 */
#ifndef _STD_COMPILE_H_
#define _STD_COMPILE_H_

#ifdef CONFIG_VM_DEBUG_INFO
#if CONFIG_VM_DEBUG_INFO == 1
#define USE_VM_DEBUG_INFO
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
