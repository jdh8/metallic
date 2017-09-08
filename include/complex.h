// This file is part of Metallic, a runtime library for WebAssembly.
//
// Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/

#ifndef _COMPLEX_H
#define _COMPLEX_H

#if __STDC_VERSION__ >= 199901L
#define complex _Complex
#define _Complex_I (__extension__ 1.0if)
#define I _Complex_I
#elif __cplusplus
namespace std { template<typename> class complex; }

extern "C" {
#endif

#define _RealScalar float
#define _SUFFIX f
#include "bits/cmathcalls.h"
#undef _RealScalar
#undef _SUFFIX

#define _RealScalar double
#define _SUFFIX
#include "bits/cmathcalls.h"
#undef _RealScalar
#undef _SUFFIX

#define _RealScalar long double
#define _SUFFIX l
#include "bits/cmathcalls.h"
#undef _RealScalar
#undef _SUFFIX

#ifdef __cplusplus
} // extern "C"
#endif

#if __STDC_VERSION__ >= 201112L && (__GNUC__ * 100 + __GNUC_MINOR__ >= 407 || __has_builtin(__builtin_complex))
#define CMPLXF(x, y) __builtin_complex((float)(x), (float)(y))
#define CMPLX(x, y) __builtin_complex((double)(x), (double)(y))
#define CMPLXL(x, y) __builtin_complex((long double)(x), (long double)(y))
#endif

#endif // complex.h
