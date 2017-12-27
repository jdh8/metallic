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
#endif

inline float __rintf(float x)
{
#ifdef FAST_ROUNDING
    return __builtin_rintf(x);
#else
    const float rectifier = 0x00800000;
    x += rectifier;
    return x - rectifier;
#endif
}

inline double __rint(double x)
{
#ifdef FAST_ROUNDING
    return __builtin_rint(x);
#else
    const double rectifier = 0x0020000000000000;
    x += rectifier;
    return x - rectifier;
#endif
}

#endif /* "round.h" */

/* vim: set ft=c: */
