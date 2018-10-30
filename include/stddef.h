/* This file is part of Metallic, a C++ library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#ifndef _STDDEF_H
#define _STDDEF_H

#include "bits/null.h"

typedef __SIZE_TYPE__ size_t;
typedef __PTRDIFF_TYPE__ ptrdiff_t;

#define offsetof(type, member) __builtin_offsetof(type, member)

#if __STDC_VERSION__ >= 201112L || __cplusplus >= 201103L
struct __max_align_t
{
#ifdef __i386__
    __float128 __g;
#endif
    long double __e;
    long long __x;
};

typedef struct __max_align_t max_align_t;
#endif

#endif /* stddef.h */
