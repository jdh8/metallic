/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <math.h>
#include <stdint.h>

static double _pade(double x)
{
    double xx = x * x;

    return x * ((xx - 105) * xx + 945) / ((15 * xx - 420) * xx + 945);
}

float tanf(float x)
{
    const double pi_2 = 1.57079632679489662;
    const float _2_pi = 0.6366197723676;
    const uint32_t thresh = 0x4F000000; /* 2 ** 31 */

    float q = nearbyintf(x * _2_pi);
    double r = x - pi_2 * q;
    double p = _pade(r);

    q /= 2;

    return nearbyintf(q) == q ? p : -1 / p;
}
