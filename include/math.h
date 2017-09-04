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
#define FP_NAN       0
#define FP_INFINITE  1
#define FP_NORMAL    2
#define FP_SUBNORMAL 3
#define FP_ZERO      4
#endif

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
} // extern "C"

inline double abs(double x) { return fabs(x); }
#endif

#if (__STDC_VERSION__ >= 199901)
# ifdef __GNUC__
# define fpclassify(x)        __builtin_fpclassify(FP_NAN, FP_INFINITE, FP_NORMAL, FP_SUBNORMAL, FP_ZERO, x)
# define isfinite(x)          __builtin_isfinite(x)
# define isinf(x)             __builtin_isinf(x)
# define isnan(x)             __builtin_isnan(x)
# define isnormal(x)          __builtin_isnormal(x)
# define signbit(x)           __builtin_signbit(x)
# define isgreater(x, y)      __builtin_isgreater(x, y)
# define isgreaterequal(x, y) __builtin_isgreaterequal(x, y)
# define isless(x, y)         __builtin_isless(x, y)
# define islessequal(x, y)    __builtin_islessequal(x, y)
# define islessgreater(x, y)  __builtin_islessgreater(x, y)
# define isunordered(x, y)    __builtin_isunordered(x, y)
# else
# define isfinite(x)          (1 / (x) && (x) == (x))
# define isinf(x)             ((x) >= 1 / 0.f || (x) <= -1 / 0.f)
# define isnan(x)             ((x) != (x))
# define isnormal(x)          (1 / (x) && isfinite(1 / (x)))
# define signbit(x)           (copysign(1, x) < 0)
# define isgreater(x, y)      ((x) > (y))
# define isgreaterequal(x, y) ((x) >= (y))
# define isless(x, y)         ((x) < (y))
# define islessequal(x, y)    ((x) <= (y))
# define islessgreater(x, y)  ((x) < (y) || (x) > (y))
# define isunordered(x, y)    ((x) != (x) || (y) != (y))
# define fpclassify(x)        (isnan(x) ? FP_NAN :       \
                               isinf(x) ? FP_INFINITE :  \
                               isnormal(x) ? FP_NORMAL : \
                               x ? FP_SUBNORMAL : FP_ZERO)
# endif
#endif /* C99 */

#endif /* math.h */
