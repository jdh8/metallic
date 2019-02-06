/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2019 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../reinterpret.h"
#include <math.h>
#include <float.h>
#include <stdint.h>

static double _kernel(double x)
{
    const double c[] = {
        1.6666666666666666663e-1,
       -2.7777777777777544901e-3,
        6.6137566135302944071e-5,
       -1.6534390729895615551e-6,
        4.1752220887981628267e-8,
       -1.0472494801471122128e-9
    };

    double xx = x * x;

    return c[0] * x + (c[1] + c[2] * x) * xx + (c[3] + c[4] * x + c[5] * xx) * (xx * xx);
}

static double _shift(double x, int64_t n)
{
    int64_t i = reinterpret(int64_t, x) + (n << 52);

    return reinterpret(double, i);
}

static double _exp(double a, double b, int64_t n)
{
    double x = a + b;
    double y = x - _kernel(x * x);
    double z = x * y / (2 - y) + b + a + 1;

    if (n < -1020)
        return 0x1p-1020 * _shift(z, n + 1020);

    return _shift(z, n);
}

double exp(double x)
{
    const double minimum = -745.1332191019412;
    const double maximum = 709.782712893385;

    const double log2e = 1.44269504088896340736;
    const double ln2[] = { 0x1.62E42FEFA4p-1, -0x1.8432A1B0E2634p-43 };

    if (x < minimum)
        return 0;

    if (x > maximum)
        return maximum * FLT_MAX;

    double n = rint(x * log2e);
    double a = x - n * ln2[0];
    double b = n * -ln2[1];

    return _exp(a, b, n);
}
