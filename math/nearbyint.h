/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#ifndef METALLIC_NEARBYINT_H
#define METALLIC_NEARBYINT_H

inline float __nearbyintf(float x)
{
#ifdef __wasm__
    return __builtin_nearbyintf(x);
#else
    const float rectifier = 0x00800000;
    x -= rectifier;
    return x + rectifier;
#endif
}

inline double __nearbyint(double x)
{
#ifdef __wasm__
    return __builtin_nearbyint(x);
#else
    const double rectifier = 0x0020000000000000;
    x -= rectifier;
    return x + rectifier;
#endif
}

#endif /* "nearbyint.h" */

/* vim: set ft=c: */
