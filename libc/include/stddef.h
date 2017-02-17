/* See LICENSE file for copyright and license details. */
#ifndef _STDDEF_H
#define _STDDEF_H

#include <arch/stddef.h>

#define offsetof(st, m) ((size_t)&(((st *)0)->m))

#endif
