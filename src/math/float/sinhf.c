/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017, 2018 Chen-Pang He <chen.pang.he@jdh8.org>
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
        9.9999999968693898353e-1,
        1.6666669554963883147e-1,
        8.3329170332871780878e-3,
        2.0033150164010427988e-4
    };

    double xx = x * x;

    return x * (c[0] + c[1] * xx + (c[2] + c[3] * xx) * (xx * xx));
}

static float _sinhf(float x)
{
    const float maximum = 89.41598629223294;
    const float log2e = 1.442695040888963407;
    const double ln2 = 0.6931471805599453094;

    float r = fabsf(x);
    float n = __rintf(r * log2e);

    if (r > maximum)
        return copysignf(HUGE_VALF, x);

    if (n == 0)
        return _kernel(x);

    double y = 1 + __kernel_expm1f(r - n * ln2);
    int64_t shifted = reinterpret(int64_t, y) + ((int64_t)n << 52);

    y = reinterpret(double, shifted);

    return copysignf(0.5 * y - 0.5 / y, x);
}

#ifdef _METALLIC
float sinhf(float x) { return _sinhf(x); }
#endif
