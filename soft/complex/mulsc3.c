/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <complex.h>
#include <math.h>

float _Complex __mulsc3(float a, float b, float c, float d)
{
    float ac = a * c;
    float ad = a * d;
    float bc = b * c;
    float bd = b * d;

    float x = ac - bd;
    float y = ad + bc;

    if (x != x && y != y) {
        int recalc = 0;

        if (isinf(a) || isinf(b)) {
            a = copysignf(!!isinf(a), a);
            b = copysignf(!!isinf(b), b);
            if (c != c) c = copysignf(0, c);
            if (d != d) d = copysignf(0, d);
            recalc = 1;
        }

        if (isinf(c) || isinf(d)) {
            c = copysignf(!!isinf(c), c);
            d = copysignf(!!isinf(d), d);
            if (a != a) a = copysignf(0, a);
            if (b != b) b = copysignf(0, b);
            recalc = 1;
        }

        if (!recalc && (isinf(ac) || isinf(bd) || isinf(ad) || isinf(bc))) {
            if (a != a) a = copysignf(0, a);
            if (b != b) b = copysignf(0, b);
            if (c != c) c = copysignf(0, c);
            if (d != d) d = copysignf(0, d);
            recalc = 1;
        }

        if (recalc) {
            x = HUGE_VALF * (a * c - b * d);
            y = HUGE_VALF * (a * d + b * c);
        }
    }

    return CMPLXF(x, y);
}
