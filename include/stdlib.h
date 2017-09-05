/* This file is part of Metallic, a C++ library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#ifndef _STDLIB_H
#define _STDLIB_H

#include "bits/specifiers.h"

typedef __SIZE_TYPE__ size_t;

#if (__cplusplus >= 201103)
#define NULL nullptr
#elif defined(__GNUG__)
#define NULL __null
#elif defined(__cplusplus)
#define NULL 0
#else
#define NULL ((void*) 0)
#endif

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#define RAND_MAX 0x7FFFFFFF

#ifdef __cplusplus
extern "C" {
#endif

_METALLIC_NORETURN void _Exit(int) _METALLIC_NOEXCEPT;
_METALLIC_NORETURN void abort(void) _METALLIC_NOEXCEPT;
_METALLIC_NORETURN void exit(int);

void* malloc(size_t);
void* calloc(size_t, size_t);
void* realloc(void*, size_t);
void free(void*);

#if (__STDC_VERSION__ >= 201112 || __cplusplus > 201402)
void* aligned_alloc(size_t, size_t);
#endif

int rand();
void srand(unsigned);
void qsort(void*, size_t, size_t, int(*)(const void*, const void*));
void* bsearch(const void*, const void*, size_t, size_t, int(*)(const void*, const void*));

typedef struct { int quot; int rem; } div_t;
typedef struct { long quot; long rem; } ldiv_t;
#if (__STDC_VERSION__ >= 199901 || __cplusplus >= 201103)
typedef struct { long long quot; long long rem; } lldiv_t;
#endif

int abs(int);
long labs(long);
#if (__STDC_VERSION__ >= 199901 || __cplusplus >= 201103)
long long llabs(long long);
#endif

inline div_t div(int x, int y) { return (div_t) { x / y, x % y }; }
inline ldiv_t ldiv(long x, long y) { return (ldiv_t) { x / y, x % y }; }
#if (__STDC_VERSION__ >= 199901 || __cplusplus >= 201103)
inline lldiv_t lldiv(long long x, long long y) { return (lldiv_t) { x / y, x % y }; }
#endif

#ifdef __cplusplus
} // extern "C"

inline long abs(long x) { return labs(x); }
#if (__cplusplus >= 201103)
inline long long abs(long long x) { return llabs(x); }
#endif

inline ldiv_t div(long x, long y) { return ldiv(x, y); }
#if (__cplusplus >= 201103)
inline lldiv_t div(long long x, long long y) { return lldiv(x, y); }
#endif

#endif /* C++ */

#endif /* stdlib.h */
