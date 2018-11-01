/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <complex.h>
#include <math.h>

static double _Complex _kernel(double a, double b, double c, double d)
{
    return CMPLX(a * c + b * d, b * c - a * d) / (c * c + d * d);
}

float _Complex __divsc3(float a, float b, float c, float d)
{
    float _Complex z = _kernel(a, b, c, d);

    if (isnan(crealf(z)) && isnan(cimagf(z))) {
        if (c == 0 && d == 0 && (a == a || b == b))
            return copysignf(INFINITY, c) * CMPLXF(a, b);

        if ((isinf(a) || isinf(b)) && isfinite(c) && isfinite(d)) {
            a = copysignf(!!isinf(a), a);
            b = copysignf(!!isinf(b), b);
            return INFINITY * CMPLXF(a * c + b * d, b * c - a * d);
        }

        if (isfinite(a) && isfinite(b) && (isinf(c) || isinf(d))) {
            c = copysignf(!!isinf(c), c);
            d = copysignf(!!isinf(d), d);
            return 0 * CMPLXF(a * c + b * d, b * c - a * d);
        }
    }

    return z;
}
