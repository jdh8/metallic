/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "kernel/expm1f.h"
#include "../nearbyint.h"
#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

static double _kernel(double x)
{
    const double c[] = {
        1.0000000755358223489,
        0.49998868839284553868,
        0.041917552126235240473
    };

    x *= x;

    return c[0] + c[1] * x + c[2] * (x * x);
}

float coshf(float x)
{
    const float maximum = 88.72283935546875;
    const float log2e = 1.442695040888963407;
    const double ln2 = 0.6931471805599453094;

    x = fabsf(x);

    if (x > maximum)
        return x * HUGE_VALF;

    float n = __nearbyintf(x * log2e);

    if (n == 0)
        return _kernel(x);

    double y = 1 + __kernel_expm1f(x - n * ln2);
    int64_t shifted = __bits(y) + ((int64_t)n << 52);

    y = __reinterpret(shifted);

    return 0.5 * y + 0.5 / y;
}
