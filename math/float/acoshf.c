/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "atanhf.h"
#include "reducef.h"
#include "quietf.h"
#include <math.h>

static double _finite(double c)
{
    const double ln2 = 0.6931471805599453094;

    int exponent;
    double s = sqrt(c * c - 1);
    double y = reducef(c + s, &exponent);

    if (exponent)
        return 2 * kernel_atanhf((y - 1) / (y + 1)) + exponent * ln2;
    else
        return 2 * kernel_atanhf((c - 1) / s);
}

float acoshf(float x)
{
    const int32_t one = 0x3F800000;
    const int32_t inf = 0x7F800000;

    int32_t i = *(int32_t*)&x;

    if (i < one)
        return quietf(x);

    if (i < inf)
        return _finite(x);

    return x;
}
