// This file is part of Metallic, a runtime library for WebAssembly.
//
// Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/

#include <tgmath.h>

Complex FUNCTION(Real a, Real b, Real c, Real d)
{
    Real ac = a * c;
    Real ad = a * d;
    Real bc = b * c;
    Real bd = b * d;

    Real x = ac - bd;
    Real y = ad + bc;

    if (x != x && y != y) {
        _Bool recalc = 0;

        if (isinf(a) || isinf(b)) {
            a = copysign(!!isinf(a), a);
            b = copysign(!!isinf(b), b);
            if (c != c) c = copysign(0, c);
            if (d != d) d = copysign(0, d);
            recalc = 1;
        }

        if (isinf(c) || isinf(d)) {
            c = copysign(!!isinf(c), c);
            d = copysign(!!isinf(d), d);
            if (a != a) a = copysign(0, a);
            if (b != b) b = copysign(0, b);
            recalc = 1;
        }

        if (!recalc && (isinf(ac) || isinf(bd) || isinf(ad) || isinf(bc))) {
            if (a != a) a = copysign(0, a);
            if (b != b) b = copysign(0, b);
            if (c != c) c = copysign(0, c);
            if (d != d) d = copysign(0, d);
            recalc = 1;
        }

        if (recalc) {
            x = INFINITY * (a * c - b * d);
            y = INFINITY * (a * d + b * c);
        }
    }
    
    return CMPLX(x, y);
}
