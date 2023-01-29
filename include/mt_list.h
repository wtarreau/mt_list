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

/* A list element, it's both a head or any element. Both pointers always point
 * to a valid list element (possibly itself for a detached element or an empty
 * list head), or are equal to MT_LIST_BUSY for a locked pointer indicating
 * that the target element is about to be modified.
 */
struct mt_list {
	struct mt_list *next;
	struct mt_list *prev;
};


/* This is the value of the locked list pointer. It is assigned to an mt_list's
 * ->next or ->prev pointer to lock the link to the other element while this
 * element is being inspected or modified.
 */
#define MT_LIST_BUSY ((struct mt_list *)1)

/* This is used to pre-initialize an mt_list element during its declaration.
 * The argument is the name of the variable being declared and being assigned
 * this value. Example:
 *
 *   struct mt_list pool_head = MT_LIST_HEAD_INIT(pool_head);
 */
#define MT_LIST_HEAD_INIT(l) { .next = &l, .prev = &l }


/* Returns a pointer of type <t> to the structure containing a member of type
 * mt_list called <m> that is accessible at address <a>. Note that <a> may be
 * the result of a function or macro since it's used only once. Example:
 *
 *   return MT_LIST_ELEM(cur_node->args.next, struct node *, args)
 */
#define MT_LIST_ELEM(a, t, m) ((t)(((const char *)(a)) - ((size_t)&((t)NULL)->m)))


/* Returns a pointer of type <t> to a structure following the element which
 * contains the list element at address <a>, which is known as member <m> in
 * struct t*. Example:
 *
 *   return MT_LIST_NEXT(args, struct node *, list);
 */
#define MT_LIST_NEXT(a, t, m) (MT_LIST_ELEM((a)->next, t, m))


/* Returns a pointer of type <t> to a structure preceeding the element which
 * contains the list element at address <a>, which is known as member <m> in
 * struct t*. Example:
 *
 *   return MT_LIST_PREV(args, struct node *, list);
 */
#define MT_LIST_PREV(a, t, m) (MT_LIST_ELEM((a)->prev, t, m))


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
