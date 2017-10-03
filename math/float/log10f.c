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
    const double log10_e2 = 0.8685889638065036553;
    const double log10_2 = 0.3010299956639811952;

    int exponent;

    x = logf_reduce(x, &exponent);

    return log10_e2 * atanhf_taylor((x - 1) / (x + 1)) + exponent * log10_2;
}

float log10f(float x)
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