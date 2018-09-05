/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "kernel/expm1f.h"
#include "../round.h"
#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

static double _kernel(double x)
{
    const double c[] = {
        9.9999999717752092082e-1,
        5.0000026035619964450e-1,
        4.1662914849658509656e-2,
        1.4061755174211695843e-3
    };

    x *= x;

    return c[0] + c[1] * x + (c[2] + c[3] * x) * (x * x);
}

static float _coshf(float x)
{
    const float maximum = 89.41598629223294;
    const float log2e = 1.442695040888963407;
    const double ln2 = 0.6931471805599453094;

    x = fabsf(x);

    if (x > maximum)
        return x * HUGE_VALF;

    float n = __rintf(x * log2e);

    if (n == 0)
        return _kernel(x);

    double y = 1 + __kernel_expm1f(x - n * ln2);
    int64_t shifted = reinterpret(int64_t, y) + ((int64_t)n << 52);

    y = reinterpret(double, shifted);

    return 0.5 * y + 0.5 / y;
}

#ifdef _METALLIC
float coshf(float x) { return _coshf(x); }
#endif
