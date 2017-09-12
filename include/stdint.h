/* This file is part of Metallic, a C++ library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#ifndef _STDINT_H
#define _STDINT_H

typedef __INTPTR_TYPE__ intptr_t;
typedef __UINTPTR_TYPE__ uintptr_t;

typedef signed char int8_t;
typedef short int16_t;
typedef int int32_t;

#ifdef __LP64__
typedef long int64_t;
#else
__extension__ typedef long long int64_t;
#endif

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

#ifdef __LP64__
typedef unsigned long uint64_t;
#else
__extension__ typedef unsigned long long uint64_t;
#endif

typedef int8_t  int_least8_t;
typedef int16_t int_least16_t;
typedef int32_t int_least32_t;
typedef int64_t int_least64_t;

typedef uint8_t  uint_least8_t;
typedef uint16_t uint_least16_t;
typedef uint32_t uint_least32_t;
typedef uint64_t uint_least64_t;

typedef signed char int_fast8_t;
typedef intptr_t    int_fast16_t;
typedef intptr_t    int_fast32_t;
typedef int64_t     int_fast64_t;

typedef unsigned char uint_fast8_t;
typedef uintptr_t     uint_fast16_t;
typedef uintptr_t     uint_fast32_t;
typedef uint64_t      uint_fast64_t;

typedef int64_t intmax_t;
typedef uint64_t uintmax_t;

#define INT8_C(c) c
#define INT16_C(c) c
#define INT32_C(c) c

#define UINT8_C(c) c
#define UINT16_C(c) c
#define UINT32_C(c) c ## U

#ifdef __LP64__
# define INT64_C(c) c ## L
# define UINT64_C(c) c ## UL
#else
# define INT64_C(c) c ## LL
# define UINT64_C(c) c ## ULL
#endif

#define INTMAX_C INT64_C
#define UINTMAX_C UINT64_C

#define INT8_MIN  (-INT8_MAX - 1)
#define INT16_MIN (-INT16_MAX - 1)
#define INT32_MIN (-INT32_MAX - 1)
#define INT64_MIN (-INT64_MAX - 1)

#define INT8_MAX  0x7F
#define INT16_MAX 0x7FFF
#define INT32_MAX 0x7FFFFFFF
#define INT64_MAX INT64_C(0x7FFFFFFFFFFFFFFF)

#define UINT8_MAX  0xFF
#define UINT16_MAX 0xFFFFU
#define UINT32_MAX 0xFFFFFFFFU
#define UINT64_MAX UINT64_C(0xFFFFFFFFFFFFFFFF)

#define INT_LEAST8_MIN  INT8_MIN
#define INT_LEAST16_MIN INT16_MIN
#define INT_LEAST32_MIN INT32_MIN
#define INT_LEAST64_MIN INT64_MIN

#ifdef __LP64__
# define INTPTR_MIN  INT64_MIN
# define INTPTR_MAX  INT64_MAX
# define UINTPTR_MAX UINT64_MAX
#else
# define INTPTR_MIN  INT32_MIN
# define INTPTR_MAX  INT32_MAX
# define UINTPTR_MAX UINT32_MAX
#endif

#define INT_FAST8_MIN  INT8_MIN
#define INT_FAST16_MIN INTPTR_MIN
#define INT_FAST32_MIN INTPTR_MIN
#define INT_FAST64_MIN INT64_MIN

#define INT_FAST8_MAX  INT8_MAX
#define INT_FAST16_MAX INTPTR_MAX
#define INT_FAST32_MAX INTPTR_MAX
#define INT_FAST64_MAX INT64_MAX

#define UINT_FAST8_MAX  UINT8_MAX
#define UINT_FAST16_MAX UINTPTR_MAX
#define UINT_FAST32_MAX UINTPTR_MAX
#define UINT_FAST64_MAX UINT64_MAX

#define INTMAX_MIN  INT64_MIN
#define INTMAX_MAX  INT64_MAX
#define UINTMAX_MAX UINT64_MAX

#define PTRDIFF_MIN INTPTR_MIN
#define PTRDIFF_MAX INTPTR_MAX

#define SIZE_MAX UINTPTR_MAX

#define SIG_ATOMIC_MIN INT32_MIN
#define SIG_ATOMIC_MAX INT32_MAX

#define WCHAR_MIN __WCHAR_MIN__
#define WCHAR_MAX __WCHAR_MAX__

#define WINT_MIN 0u
#define WINT_MAX INT32_MAX

#endif /* stdint.h */
