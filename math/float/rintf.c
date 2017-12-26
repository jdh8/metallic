/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../rint.h"
#include <math.h>
#include <float.h>

float rintf(float x)
{
#if defined(__wasm__) || defined(__FAST_MATH__)
    return __rintf(x);
#else
    float rectifier = copysignf(0x00800000, x);
    float y = x - rectifier;

    return copysignf(y + rectifier, x);
#endif
}
