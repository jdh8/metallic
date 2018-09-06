/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017, 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <math.h>

float rintf(float x)
{
#if defined(__wasm__) || defined(__AVX__) || defined(__SSE4_1__)
    return __builtin_rintf(x);
#else
    const float rectifier = 0x00800000;
    float y = fabsf(x) + rectifier;
    return copysignf(y - rectifier, x);
#endif
}

#ifdef __wasm__
float nearbyintf(float) __attribute__((alias("rintf")));
#endif
