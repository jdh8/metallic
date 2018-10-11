/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "finite/cashf.h"

static float _Complex _casinhf(float x, float y)
{
    const double pi = 3.14159265358979323846;

    if (x == INFINITY)
        return CMPLXF(INFINITY, y == INFINITY ? pi / 4 : 0 * y);

    if (y == INFINITY)
        return CMPLXF(INFINITY, x == x ? pi / 2 : x);

    if (isunordered(x, y))
        return CMPLXF(NAN, y ? NAN : 0);

    return _cashf(x, y, _carg);
}