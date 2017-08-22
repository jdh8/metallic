/* This file is part of Metallic, a C++ library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#define MB_LEN_MAX 16

#define CHAR_BIT 8

#define SCHAR_MIN (-128)
#define SCHAR_MAX 127
#define UCHAR_MAX 255

#ifdef __CHAR_UNSIGNED__
# define CHAR_MIN 0
# define CHAR_MAX UCHAR_MAX
#else
# define CHAR_MIN SCHAR_MIN
# define CHAR_MAX SCHAR_MAX
#endif

#define SHRT_MIN  (-32768)
#define SHRT_MAX  32767
#define USHRT_MAX 65535

#define INT_MIN  (-INT_MAX - 1)
#define INT_MAX  0x7FFFFFFF
#define UINT_MAX 0xFFFFFFFFU

#define LONG_MIN  (-LONG_MAX - 1)

#ifdef __LP64__
# define LONG_MAX  0x7FFFFFFFFFFFFFFF
# define ULONG_MAX 0xFFFFFFFFFFFFFFFFU
#else
# define LONG_MAX  0x7FFFFFFF
# define ULONG_MAX 0xFFFFFFFFU
#endif

#define LLONG_MIN  (-LLONG_MAX - 1)
#define LLONG_MAX  0x7FFFFFFFFFFFFFFF
#define ULLONG_MAX 0xFFFFFFFFFFFFFFFFU
