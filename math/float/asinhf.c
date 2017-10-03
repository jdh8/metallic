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
    const double ln2 = 0.6931471805599453094;

    int exponent;
    double cosh = sqrt(x * x + 1);
    double y = logf_reduce(x + cosh, &exponent);

    if (exponent)
        return 2 * atanhf_taylor((y - 1) / (y + 1)) + exponent * ln2;
    else
        return atanhf_taylor(x / cosh); /* TODO not precise enough */
}

float asinhf(float x)
{
    if (x - x == 0)
        return _finite(x);

    return x;
}
