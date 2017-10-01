/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "atanhf/taylor.h"
#include "logf/reduce.h"
#include "quietf.h"
#include <math.h>

static double _finite(double x)
{
    const double ln2 = 0.6931471805599452862;
    int exponent;

    x = logf_reduce(x, &exponent);

    return 2 * atanhf_taylor((x - 1) / (x + 1)) + exponent * ln2;
}

float logf(float x)
{
    const int32_t inf = 0x7F800000;
    
    if (x == 0)
        return -HUGE_VALF;

    int32_t i = *(int32_t*)&x;

    if (i < 0)
        return quietf(x);

    if (i < inf)
        return _finite(x);

    return x;
}
