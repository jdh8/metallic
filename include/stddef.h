/* This file is part of Metallic, a C++ library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
typedef __SIZE_TYPE__ size_t;
typedef __PTRDIFF_TYPE__ ptrdiff_t;

#if (__cplusplus >= 201103)
# define NULL nullptr
#elif defined(__cplusplus)
# define NULL 0
#else
# define NULL ((void*) 0)
#endif

#define offsetof(type, member) __builtin_offsetof(type, member)

#if __cplusplus >= 201103

typedef decltype(nullptr) nullptr_t;

class max_align_t
{
# ifdef __i386__
    __float128 __g;
# endif
    long double __e;
    long long __x;
};

#endif /* C++11 */

#if __cplusplus > 201402
enum class byte: unsigned char {};
#endif /* C++17 */
