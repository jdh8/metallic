/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "kernel/expm1f.h"
#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

static double _kernel(double x)
{
    const double c[] = {
        1.0000000107805563820,
        1.6666505200553427403e-1,
        8.3691534741249721668e-3
    };

    double xx = x * x;

    return x * (c[0] + c[1] * xx + c[2] * (xx * xx));
}

float sinhf(float x)
{
    const float maximum = 88.72283935546875;
    const float log2e = 1.442695040888963407;
    const double ln2 = 0.6931471805599453094;

    if (fabsf(x) > maximum)
        return x * HUGE_VALF;

    float n = nearbyintf(x * log2e);

    if (n == 0)
        return _kernel(x);

    double y = 1 + __kernel_expm1f(x - n * ln2);
    int64_t shifted = __bits(y) + ((int64_t)n << 52);

    y = __reinterpret(shifted);

    return 0.5 * y - 0.5 / y;
}
