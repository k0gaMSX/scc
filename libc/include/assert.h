/* See LICENSE file for copyright and license details. */
#ifndef _ASSERT_H
#define _ASSERT_H

void __assert(int status, char *exp, char *file, long line);

#ifndef NDEBUG
# define assert(exp) __assert(exp, #exp, __FILE__, __LINE__)
#else
# define assert(exp) ((void)0)
#endif

#endif
