/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#ifndef _COMPLEX_H
#define _COMPLEX_H

#if __cplusplus
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

#ifdef __clang__
#define __CMPLX(T, x, y) (T _Complex){ x, y }
#elif __GNUC__
#define __CMPLX(T, x, y) __builtin_complex((T)(x), (T)(y))
#else
#define __CMPLX(T, x, y) (union { T _Complex __z; T __xy[2]; }){.__xy = {(x), (y)}}.__z
#endif

#if __STDC_VERSION__ >= 199901L
#if __GNUC__
static const float _Complex _Complex_I = __extension__ 1.0if;
#define _Complex_I (__extension__ 1.0if)
#elif
static const float _Complex _Complex_I = __CMPLX(float, 0, 1);
#define _Complex_I __CMPLX(float, 0, 1)
#endif

#define I _Complex_I
#define complex _Complex
#endif

#if __STDC_VERSION__ >= 201112L
#define CMPLXF(x, y) __CMPLX(float, x, y)
#define CMPLX(x, y)  __CMPLX(double, x, y)
#define CMPLXL(x, y) __CMPLX(long double, x, y)
#endif

#endif /* complex.h */
