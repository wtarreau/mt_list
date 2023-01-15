/*
 * include/mt_list.h
 *
 * Multi-thread aware circular lists.
 *
 * Copyright (C) 2018-2023 Willy Tarreau
 * Copyright (C) 2018-2023 Olivier Houchard
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef _MT_LIST_H
#define _MT_LIST_H

#include <inttypes.h>
#include <stddef.h>

/* set NOINLINE to forcefully disable user functions inlining */
#if defined(NOINLINE)
#define MT_INLINE __attribute__((noinline))
#else
#define MT_INLINE inline
#endif

/* This is used to prevent the compiler from knowing the origin of the
 * variable, and sometimes avoid being confused about possible null-derefs
 * that it sometimes believes are possible after pointer casts.
 */
#define MT_ALREADY_CHECKED(p) do { asm("" : "=rm"(p) : "0"(p)); } while (0)

/* This function relaxes the CPU during contention. It is meant to be
 * architecture-specific and may even be OS-specific, and always exists in a
 * generic version. It should return a non-null integer value that can be used
 * as a boolean in while() loops.
 */
static inline long mt_list_cpu_relax(void)
{
	return 1;
}

#endif /* _MT_LIST_H */
