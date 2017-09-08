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

#if __GNUC__ * 100 + __GNUC_MINOR__ >= 303
#define NAN      __builtin_nanf("")
#define INFINITY __builtin_inff()
#define HUGE_VALF __builtin_huge_valf()
#define HUGE_VAL  __builtin_huge_val()
#define HUGE_VALL __builtin_huge_vall()
#define FP_ILOGBNAN __INT_MAX__
#define FP_ILOGB0   (-__INT_MAX__ - (-1 & 3 == 3))  // INT_MIN
#else
#define NAN (0.0f / 0.0f)
#define INFINITY 9e999f
#define HUGE_VALF INFINITY
#define HUGE_VAL  ((double)INFINITY)
#define HUGE_VALL ((long double)INFINITY)
#define FP_ILOGBNAN ((int)((unsigned)-1 / 2))       // INT_MAX
#define FP_ILOGB0   (-FP_ILOGBNAN - (-1 & 3 == 3))  // INT_MIN
#endif
#endif /* C99 or C++11 */

#if __STDC_VERSION__ >= 199901L
#if __GNUC__ >= 4
#define signbit(x) __builtin_signbit(x)
#else
#define signbit(x) (sizeof(x - 0.0) > sizeof(long double) ? copysignl(1, x) < 0 : copysign(1, x) < 0)
#endif

#if __GNUC__ * 100 + __GNUC_MINOR__ >= 404 || __has_builtin(__builtin_fpclassify)
#define fpclassify(x)        __builtin_fpclassify(FP_NAN, FP_INFINITE, FP_NORMAL, FP_SUBNORMAL, FP_ZERO, x)
#define isfinite(x)          __builtin_isfinite(x)
#define isinf(x)             __builtin_isinf(x)
#define isnan(x)             __builtin_isnan(x)
#define isnormal(x)          __builtin_isnormal(x)
#define isgreater(x, y)      __builtin_isgreater(x, y)
#define isgreaterequal(x, y) __builtin_isgreaterequal(x, y)
#define isless(x, y)         __builtin_isless(x, y)
#define islessequal(x, y)    __builtin_islessequal(x, y)
#define islessgreater(x, y)  __builtin_islessgreater(x, y)
#define isunordered(x, y)    __builtin_isunordered(x, y)
#else
#define isnan(x)             ((x) != (x))
#define isunordered(x, y)    (isnan(x) || isnan(y))
#define isgreater(x, y)      (!isunordered(x, y) && (x) > (y))
#define isgreaterequal(x, y) (!isunordered(x, y) && (x) >= (y))
#define isless(x, y)         (!isunordered(x, y) && (x) < (y))
#define islessequal(x, y)    (!isunordered(x, y) && (x) <= (y))
#define islessgreater(x, y)  (!isunordered(x, y) && (x) != (y))
#define isfinite(x)          ((x) - (x) == 0)
#define isinf(x)             ((x) == (x) && (x) - (x))
#define isnormal(x)          (isfinite(x) && isfinite(1 / (x)))
#define fpclassify(x)        (isfinite(x) ? (x) ? (isfinite(1 / (x)) ? FP_NORMAL : FP_SUBNORMAL) : FP_ZERO \
                                          : (x) == (x) ? FP_INFINITE : FP_NAN)
#endif
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
#endif

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* math.h */
