/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "atanhf_pade.h"
#include "logf_reduce.h"
#include <math.h>

static double _finite(double s)
{
    const double ln2 = 0.6931471805599453094;

    int exponent;
    double c = sqrt(s * s + 1);
    double y = logf_reduce(c + s, &exponent);

    if (exponent)
        return 2 * atanhf_pade((y - 1) / (y + 1)) + exponent * ln2;
    else
        return 2 * atanhf_pade(s / (c + 1));
}

float asinhf(float x)
{
    if (x - x == 0)
        return _finite(x);

    return x;
}
