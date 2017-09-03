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

float fabsf(float);
double fabs(double);
long double fabsl(long double);

#ifdef __cplusplus
} // extern "C"

inline float abs(float x) { return fabsf(x); }
inline double abs(double x) { return fabs(x); }
inline long double abs(long double x) { return fabsl(x); }
#endif

#endif /* math.h */
