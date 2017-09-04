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

#endif /* math.h */
