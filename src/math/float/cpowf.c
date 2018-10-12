/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "clogf.h"
#include "cisf.h"
#include "expf.h"

float _Complex cpowf(float _Complex base, float _Complex exp)
{
    double _Complex z = exp * _clogf(base);
    double x = z;
    double y = cimag(z);

    if (y == 0)
        return CMPLX(_expf(x), y);

    if (y - y) {
        if (x == INFINITY)
            return CMPLX(x, y - y);

        if (x == -INFINITY)
            return 0;
    }

    return _expf(x) * _cisf(y);
}
