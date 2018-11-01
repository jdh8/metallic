/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "complex.h"
#include <tgmath.h>

Complex FUNCTION(Real a, Real b, Real c, Real d)
{
    Real logbw = logb(fmax(fabs(c), fabs(d)));
    int scale = 0;

    if (isfinite(logbw)) {
        scale = -logbw;
        c = scalbn(c, scale);
        d = scalbn(d, scale);
    }

    Real denominator = c * c + d * d;
    Real x = scalbn((a * c + b * d) / denominator, scale);
    Real y = scalbn((b * c - a * d) / denominator, scale);

    if (x != x && y != y) {
        if (denominator == 0 && (a == a || b == b))
            return copysign(INFINITY, c) * _complex(a, b);

        if ((isinf(a) || isinf(b)) && isfinite(c) && isfinite(d)) {
            a = copysignf(!!isinf(a), a);
            b = copysignf(!!isinf(b), b);
            return INFINITY * _complex(a * c + b * d, b * c - a * d);
        }

        if (logbw == INFINITY && isfinite(a) && isfinite(b)) {
            c = copysignf(!!isinf(c), c);
            d = copysignf(!!isinf(d), d);
            return 0 * _complex(a * c + b * d, b * c - a * d);
        }
    }
    
    return _complex(x, y);
}
