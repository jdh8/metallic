#ifndef _BITS_SPECIFIERS_H
#define _BITS_SPECIFIERS_H

#if __cplusplus >= 201103L
#define _NORETURN [[noreturn]]
#elif __STDC_VERSION__ >= 201112L
#define _NORETURN _Noreturn
#elif __GNUC__
#define _NORETURN __attribute__((__noreturn__))
#else
#define _NORETURN
#endif

#if __cplusplus >= 201103L
#define _NOEXCEPT noexcept
#else
#define _NOEXCEPT
#endif

#endif /* bits/specifiers.h */
