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

#define SCHAR_MIN (-SCHAR_MAX - (-1 == ~0))
#define SCHAR_MAX __SCHAR_MAX__
#define UCHAR_MAX (SCHAR_MAX * 2u + 1)

#if -'\1' < 0
# define CHAR_MIN SCHAR_MIN
# define CHAR_MAX SCHAR_MAX
#else
# define CHAR_MIN '\0'
# define CHAR_MAX UCHAR_MAX
#endif

#define SHRT_MIN  (-SHRT_MAX - (-1 == ~0))
#define SHRT_MAX  __SHRT_MAX__
#define USHRT_MAX (SHRT_MAX * 2u + 1)

#define INT_MIN  (-INT_MAX - (-1 == ~0))
#define INT_MAX  __INT_MAX__
#define UINT_MAX (INT_MAX * 2u + 1)

#define LONG_MIN  (-LONG_MAX - (-1L == ~0L))
#define LONG_MAX  __LONG_MAX__
#define ULONG_MAX (LONG_MAX * 2u + 1)

#define LLONG_MIN  (-LLONG_MAX - (-1LL == ~0LL))
#define LLONG_MAX  __LONG_LONG_MAX__
#define ULLONG_MAX (LLONG_MAX * 2u + 1)

#endif /* limits.h */
