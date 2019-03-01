#ifndef _STDARG_H
#define _STDARG_H

typedef __builtin_va_list va_list;

#define va_start(list, antecedent) __builtin_va_start(list, antecedent)
#define va_arg(list, type) __builtin_va_arg(list, type)
#define va_end(list) __builtin_va_end(list)

#if __STDC_VERSION__ >= 199901L || __cplusplus >= 201103L
#define va_copy(destination, source) __builtin_va_copy(destination, source)
#endif

#endif
