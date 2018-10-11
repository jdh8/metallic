/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "finite/cashf.h"

static double _atan2(double y, double x)
{
    return _carg(x, y);
}

static double _Complex _cacoshf(float x, float y)
{
    const double pi = 3.14159265358979323846;

    if (x == INFINITY)
        return CMPLX(INFINITY, y == INFINITY ? pi / 4 : 0 * y);

    if (y == INFINITY)
        return CMPLX(INFINITY, x == x ? pi / 2 : x);

    if (isunordered(x, y))
        return CMPLX(NAN, x ? NAN : pi / 2);

    return _cashf(y, x, _atan2);
}
