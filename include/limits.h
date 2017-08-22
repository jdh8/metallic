/* This file is part of Metallic, a C++ library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#define MB_LEN_MAX 16

#ifdef __CHAR_BIT__
#define CHAR_BIT __CHAR_BIT__
#else
#define CHAR_BIT 8
#endif

#define SCHAR_MIN (-SCHAR_MAX - 1)
#define SCHAR_MAX ((signed char)(UCHAR_MAX / 2))
#define UCHAR_MAX ((unsigned char)-1)

#if (char)-1 < 0
# define CHAR_MIN SCHAR_MIN
# define CHAR_MAX SCHAR_MAX
#else
# define CHAR_MIN '\0'
# define CHAR_MAX UCHAR_MAX
#endif

#define SHRT_MIN  (-SHRT_MAX - 1)
#define SHRT_MAX  ((short)(USHRT_MAX / 2))
#define USHRT_MAX ((unsigned short)-1)

#define INT_MIN  (-INT_MAX - 1)
#define INT_MAX  ((int)(-1u / 2))
#define UINT_MAX (-1u)

#define LONG_MIN  (-LONG_MAX - 1)
#define LONG_MAX  ((long)(-1UL / 2))
#define ULONG_MAX (-1UL)

#define LLONG_MIN  (-LLONG_MAX - 1)
#define LLONG_MAX  ((long long)(-1ULL / 2))
#define ULLONG_MAX (-1ULL)
