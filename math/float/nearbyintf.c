/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../nearbyint.h"
#include <math.h>
#include <float.h>

float nearbyintf(float x)
{
#if defined(__wasm__) || defined(__FAST_MATH__)
    return __nearbyintf(x);
#else
    switch (FLT_ROUNDS) {
        case 0:
            return truncf(x);
        case 2:
            return ceilf(x);
        case 3:
            return floorf(x);
    }

    return copysignf(__nearbyintf(fabsf(x)), x);
#endif
}
