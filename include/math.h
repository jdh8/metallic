/* This file is part of Metallic, a C++ library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#ifndef _MATH_H
#define _MATH_H

#if (__STDC_VERSION__ >= 199901 || __cplusplus >= 201103)

typedef float float_t;
typedef double double_t;

#define FP_NAN       0
#define FP_INFINITE  1
#define FP_NORMAL    2
#define FP_SUBNORMAL 3
#define FP_ZERO      4

#define HUGE_VALF __builtin_huge_valf()
#define HUGE_VAL  __builtin_huge_val()
#define HUGE_VALL __builtin_huge_vall()

#define INFINITY  __builtin_inff()
#define NAN       __builtin_nanf("")

#define MATH_ERRNO     1
#define MATH_ERREXCEPT 2

#if defined(__FAST_MATH__) || defined(__wasm__)
#define math_errhandling 0
#else
#define math_errhandling (MATH_ERRNO | MATH_ERREXCEPT)
#endif

#endif /* C99 or C++11 */

#if (__STDC_VERSION__ >= 199901)
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
#endif /* C99 */

#define _Scalar double
#include "bits/mathcall.h"
#undef _Scalar

#define _Scalar float
#define _SUFFIX f
#include "bits/mathcall.h"
#undef _Scalar
#undef _SUFFIX

#define _Scalar long double
#define _SUFFIX l
#include "bits/mathcall.h"
#undef _Scalar
#undef _SUFFIX

#endif /* math.h */
