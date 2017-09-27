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

float exp2f(float x)
{
    const double ln2 = 0.6931471805599452862;
    const double b[] = { 120, 60, 12, 1 };

    if (x < -150)
        return 0;

    if (x > 128)
        return x * HUGE_VALF;

    float n = nearbyintf(x);

    double a = (x - n) * ln2;
    double a2 = a * a;
    double u = a * (b[3] * a2 + b[1]);
    double v = b[2] * a2 + b[0];
    double y = (v + u) / (v - u);

    int64_t shifted = *(int64_t*)&y + ((int64_t) n << 52);

    return *(double*)&shifted;
}
