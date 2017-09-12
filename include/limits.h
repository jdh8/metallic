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

/* Minima and maxima can be found in a cross-platform way due to the following
 * facts:
 *
 * 1. Unsigned integers wrap around, so (Unsigned)-1 is the maximum.
 * 2. Maximum of the signed counterpart is (Signed)((Unsigned)-1 / 2).
 * 3. Signed integer representation must be 2's complement, 1's complement,
 *    or sign-magnitude.
 * 3.a. Minimum is (-maximum - 1) for 2's complement or -maximum otherwise.
 * 3.b. -1 == ~0 is true iff the representation is 2's complement.
 */
#define SCHAR_MIN (-SCHAR_MAX - (-(signed char)1 == ~(signed char)0))
#define SCHAR_MAX ((signed char)(UCHAR_MAX / 2))
#define UCHAR_MAX ((unsigned char)-1)

#if (char)-1 < 0  /* Signed */
# define CHAR_MIN SCHAR_MIN
# define CHAR_MAX SCHAR_MAX
#else
# define CHAR_MIN '\0'
# define CHAR_MAX UCHAR_MAX
#endif

#define SHRT_MIN  (-SHRT_MAX - (-(short)1 == ~(short)0))
#define SHRT_MAX  ((short)(USHRT_MAX / 2))
#define USHRT_MAX ((unsigned short)-1)

#define INT_MIN  (-INT_MAX - (-1 == ~0))
#define INT_MAX  ((int)(-1u / 2))
#define UINT_MAX (-1u)

#define LONG_MIN  (-LONG_MAX - (-1L == ~0L))
#define LONG_MAX  ((long)(-1UL / 2))
#define ULONG_MAX (-1UL)

#define LLONG_MIN  (-LLONG_MAX - (-1LL == ~0LL))
#define LLONG_MAX  ((long long)(-1ULL / 2))
#define ULLONG_MAX (-1ULL)

/* Usually CHAR_BIT is a power of 2.  The other edge cases are covered by
 * popcount(UCHAR_MAX) via 64-bit multiplication.  The detailed algorithm was
 * found at
 * https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSet64
 */
#ifdef __CHAR_BIT__
#define CHAR_BIT __CHAR_BIT__
#elif UCHAR_MAX == 0xFF
#define CHAR_BIT 8
#elif UCHAR_MAX == 0xFFFF
#define CHAR_BIT 16
#elif UCHAR_MAX == 0xFFFFFFFFU
#define CHAR_BIT 32
#elif UCHAR_MAX <= 0xFFFFFFFFU
#define CHAR_BIT (((UCHAR_MAX & 0xFFF) * 0x1001001001001ULL & 0x84210842108421ULL) % 0x1F \
     + (((UCHAR_MAX & 0xFFF000) >> 12) * 0x1001001001001ULL & 0x84210842108421ULL) % 0x1F \
                  + ((UCHAR_MAX >> 24) * 0x1001001001001ULL & 0x84210842108421ULL) % 0x1F)
#else
#error Narrow character with > 32 bits is unsupported for a GCC-incompatible compiler.
#endif

/* Unicode may consume 32 bits for a character. */
#define MB_LEN_MAX (32 / CHAR_BIT + !!(32 % CHAR_BIT))

#endif /* limits.h */
