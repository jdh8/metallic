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
# define isgreater(x, y)      __builtin_isgreater(x, y)
# define isgreaterequal(x, y) __builtin_isgreaterequal(x, y)
# define isless(x, y)         __builtin_isless(x, y)
# define islessequal(x, y)    __builtin_islessequal(x, y)
# define islessgreater(x, y)  __builtin_islessgreater(x, y)
# define isunordered(x, y)    __builtin_isunordered(x, y)
# else
# define isgreater(x, y)      ((x) > (y))
# define isgreaterequal(x, y) ((x) >= (y))
# define isless(x, y)         ((x) < (y))
# define islessequal(x, y)    ((x) <= (y))
# define islessgreater(x, y)  ((x) < (y) || (x) > (y))
# define isunordered(x, y)    ((x) != (x) || (y) != (y))
# endif
#endif

#endif /* math.h */
