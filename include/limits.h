/* This file is part of Metallic, a C++ library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#ifndef _LIMITS_H
#define _LIMITS_H

#define CHAR_BIT __CHAR_BIT__

/* Unicode may consume 32 bits for a character. */
#define MB_LEN_MAX (32 / CHAR_BIT + !!(32 % CHAR_BIT))

/* Minima and maxima can be found in a cross-platform way due to the following
 * facts:
 *
 * 1. Unsigned integers wrap around, so (Unsigned)-1 is the maximum.
 * 2. Signed integer representation must be 2's complement, 1's complement,
 *    or sign-magnitude.
 * 2.a. Minimum is (-maximum - 1) for 2's complement or -maximum otherwise.
 * 2.b. -1 == ~0 is true iff the representation is 2's complement.
 */
#define SCHAR_MIN (-SCHAR_MAX - (-(signed char)1 == ~(signed char)0))
#define SCHAR_MAX __SCHAR_MAX__
#define UCHAR_MAX ((unsigned char)-1)

#if (char)-1 < 0  /* Signed */
# define CHAR_MIN SCHAR_MIN
# define CHAR_MAX SCHAR_MAX
#else
# define CHAR_MIN '\0'
# define CHAR_MAX UCHAR_MAX
#endif

#define SHRT_MIN  (-SHRT_MAX - (-(short)1 == ~(short)0))
#define SHRT_MAX  __SHRT_MAX__
#define USHRT_MAX ((unsigned short)-1)

#define INT_MIN  (-INT_MAX - (-1 == ~0))
#define INT_MAX  __INT_MAX__
#define UINT_MAX (-1u)

#define LONG_MIN  (-LONG_MAX - (-1L == ~0L))
#define LONG_MAX  __LONG_MAX__
#define ULONG_MAX (-1UL)

#define LLONG_MIN  (-LLONG_MAX - (-1LL == ~0LL))
#define LLONG_MAX  __LONG_LONG_MAX__
#define ULLONG_MAX (-1ULL)

#endif /* limits.h */
