/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#ifndef METALLIC_ROUND_H
#define METALLIC_ROUND_H

#if defined(__wasm__) || defined(__AVX__) || defined(__SSE4_1__)

#define FAST_ROUNDING
#define __rintf __builtin_rintf
#define __rint  __builtin_rint

#else

#include <math.h>

inline float __rintf(float x)
{
    const float rectifier = 0x00800000;
    float y = fabsf(x) + rectifier;
    return copysignf(y - rectifier, x);
}

inline double __rint(double x)
{
    const double rectifier = 0x0010000000000000;
    double y = fabs(x) + rectifier;
    return copysign(y - rectifier, x);
}

#endif

#endif /* "round.h" */

/* vim: set ft=c: */
