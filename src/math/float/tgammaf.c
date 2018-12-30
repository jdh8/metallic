/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "exp2f.h"
#include "finite/log2f.h"
#include "finite/sinpif.h"
#include <math.h>

static double _sum(double z)
{
    const double p[] = {
        2.5066282972608683788,
        93.724008703110370647,
       -85.030278048785721233,
        15.313090747320992728,
       -0.2376573970640603182
    };

    return p[4] / (z + 4) + p[3] / (z + 3) + (p[2] / (z + 2) + p[1] / (z + 1)) + p[0];
}

static double _gamma1p(double z)
{
    const double log2e = 1.442695040888963407;

    double base = 4.8644453082153116114 + z;

    return _exp2f((0.5 + z) * _log2f(base) - log2e * base) * _sum(z);
}

float tgammaf(float z)
{
    const double pi = 3.14159265358979324;

    if (z == 0)
        return copysignf(INFINITY, z);

    if (z == INFINITY)
        return INFINITY;

    if (z < 0.5f) {
        if (rintf(z) == z)
            return NAN;
        return pi / (_sinpif(z) * _gamma1p(-z));
    }

    return _gamma1p(z - 1.0);
}
