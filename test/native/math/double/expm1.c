/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "unary.h"
#include <assert.h>
#include <math.h>

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

static double _expm1(double x)
{
    return 2 * x / (_kernel(x * x) - x + 2);
}

static double _exp(double x)
{
    double y = x - _kernel(x * x);

    return x * y / (2 - y) + x + 1;
}

int main(void)
{
    for (int64_t i = 0; i < 0x3FD62E42FEFA39EF; i += 0x000000BB8644867C) {
        double x = reinterpret(double, i);
        verify(approx(expm1l(x), _expm1(x), 1), x);
        verify(approx(expm1l(-x), _expm1(-x), 1), x);
    }
}
