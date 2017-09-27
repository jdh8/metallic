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

#define NAN      __builtin_nanf("")
#define INFINITY __builtin_inff()
#define HUGE_VALF __builtin_huge_valf()
#define HUGE_VAL  __builtin_huge_val()
#define HUGE_VALL __builtin_huge_vall()
#define FP_ILOGBNAN __INT_MAX__
#define FP_ILOGB0   (-__INT_MAX__ - (-1 == ~0))  // INT_MIN

#define FP_NAN       0
#define FP_INFINITE  1
#define FP_ZERO      2
#define FP_SUBNORMAL 3
#define FP_NORMAL    4

#define MATH_ERRNO     1
#define MATH_ERREXCEPT 2

#if defined(__wasm__) || defined(__FAST_MATH__)
#define math_errhandling 0
#else
#define math_errhandling MATH_ERREXCEPT
#endif
#endif /* C99 or C++11 */

#if __STDC_VERSION__ >= 199901L
#define fpclassify(x) __builtin_fpclassify(FP_NAN, FP_INFINITE, FP_NORMAL, FP_SUBNORMAL, FP_ZERO, x)
#define isfinite(x)   __builtin_isfinite(x)
#define isinf(x)      __builtin_isinf(x)
#define isnan(x)      __builtin_isnan(x)
#define isnormal(x)   __builtin_isnormal(x)
#define signbit(x)    __builtin_signbit(x)
#define isgreater(x, y)      __builtin_isgreater(x, y)
#define isgreaterequal(x, y) __builtin_isgreaterequal(x, y)
#define isless(x, y)         __builtin_isless(x, y)
#define islessequal(x, y)    __builtin_islessequal(x, y)
#define islessgreater(x, y)  __builtin_islessgreater(x, y)
#define isunordered(x, y)    __builtin_isunordered(x, y)
#endif /* C99 */

#ifdef __cplusplus
extern "C" {
#endif

#define _Scalar double
#define _SUFFIX
#include "bits/mathcalls.h"
#undef _Scalar
#undef _SUFFIX

#if __STDC_VERSION__ >= 199901L || defined(__cplusplus)
#define _Scalar float
#define _SUFFIX f
#include "bits/mathcalls.h"
#undef _Scalar
#undef _SUFFIX

#define _Scalar long double
#define _SUFFIX l
#include "bits/mathcalls.h"
#undef _Scalar
#undef _SUFFIX

#include "bits/mathinline.h"
#endif

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* math.h */
