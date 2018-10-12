/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "cisf.h"
#include "expf.h"

float _Complex cexpf(float _Complex z)
{
    float x = z;
    float y = cimagf(z);

    if (y == 0)
        return CMPLXF(_expf(x), y);

    if (y - y) {
        if (x == INFINITY)
            return CMPLXF(x, y - y);

        if (x == -INFINITY)
            return 0;
    }

    return _expf(x) * _cisf(y);
}
