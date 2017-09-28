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

float expf(float x)
{
    const float minimum = -103.972084045410;
    const float maximum = 88.72283935546875;

    const float log2e = 1.44269502163;
    const double ln2 = 0.6931471805599452862;
    const int b[] = { 1680, 840, 180, 20, 1 };

    if (x < minimum)
        return 0;

    if (x > maximum)
        return x * HUGE_VALF;

    float n = nearbyintf(x * log2e);

    double a = x - n * ln2;
    double a2 = a * a;
    double u = a * (b[3] * a2 + b[1]);
    double v = (b[4] * a2 + b[2]) * a2 + b[0];
    double y = (v + u) / (v - u);

    int64_t shifted = *(int64_t*)&y + ((int64_t) n << 52);

    return *(double*)&shifted;
}
