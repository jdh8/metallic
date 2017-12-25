/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "kernel/atanf.h"
#include <math.h>

inline double __atan2f(float y, float x)
{
    const double pi = 3.14159265358979324;

    float absy = fabsf(y);
    float absx = fabsf(x);

    if (absy > absx)
        return copysign(pi / 2, y) - __kernel_atanf(x / (double)y);

    double sharp = absy == absx ? copysign(pi / 4, y) : __kernel_atanf(y / (double)absx);

    return signbit(x) ? copysign(pi, y) - sharp : sharp;
}
