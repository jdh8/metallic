/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#ifndef _MATH_H
#define _MATH_H

#if __STDC_VERSION__ >= 199901L || __cplusplus >= 201103L
typedef float float_t;
typedef double double_t;

#define NAN (0.0f / 0.0f)
#define INFINITY 9e999f

#define HUGE_VALF INFINITY
#define HUGE_VAL  ((double)INFINITY)
#define HUGE_VALL ((long double)INFINITY)

#define MATH_ERRNO  1
#define MATH_ERREXCEPT 2

#if defined(__wasm__) || defined(__FAST_MATH__)
#define math_errhandling 0
#else
#define math_errhandling MATH_ERREXCEPT
#endif

#define FP_ILOGBNAN ((int)((unsigned)-1 / 2))       /* INT_MAX */
#define FP_ILOGB0   (-FP_ILOGBNAN - (-1 & 3 == 3))  /* INT_MIN */

#define FP_NAN       0
#define FP_INFINITE  1
#define FP_ZERO      2
#define FP_SUBNORMAL 3
#define FP_NORMAL    4
#endif

#if __STDC_VERSION__ >= 199901L
#define fpclassify(x)        __builtin_fpclassify(FP_NAN, FP_INFINITE, FP_NORMAL, FP_SUBNORMAL, FP_ZERO, x)
#define isfinite(x)          __builtin_isfinite(x)
#define isinf(x)             __builtin_isinf(x)
#define isnan(x)             __builtin_isnan(x)
#define isnormal(x)          __builtin_isnormal(x)
#define signbit(x)           __builtin_signbit(x)
#define isgreater(x, y)      __builtin_isgreater(x, y)
#define isgreaterequal(x, y) __builtin_isgreaterequal(x, y)
#define isless(x, y)         __builtin_isless(x, y)
#define islessequal(x, y)    __builtin_islessequal(x, y)
#define islessgreater(x, y)  __builtin_islessgreater(x, y)
#define isunordered(x, y)    __builtin_isunordered(x, y)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define _Scalar float
#define _SUFFIX
#include "bits/mathcalls.h"
#undef _Scalar
#undef _SUFFIX

#define _Scalar double
#define _SUFFIX
#include "bits/mathcalls.h"
#undef _Scalar
#undef _SUFFIX

#define _Scalar long double
#define _SUFFIX
#include "bits/mathcalls.h"
#undef _Scalar
#undef _SUFFIX

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* math.h */
