/* See LICENSE file for copyright and license details. */
#ifndef _STDARG_H_
#define _STDARG_H_

typedef struct {
    unsigned gp_offset;
    unsigned fp_offset;
    void *overflow_arg_area;
    void *reg_save_area;
} __va_elem;

typedef __va_elem va_list[1];

#define va_start(ap, last) __builtin_va_start((ap), (last))
#define va_end(ap) __builtin__va_end(ap)
#define va_copy(to, from) __builtin_va_copy((to), (from))

#endif
