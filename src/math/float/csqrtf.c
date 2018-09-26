/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "csqrt.h"
#include <complex.h>
#include <math.h>

float _Complex csqrtf(float _Complex z)
{
    float x = z;
    float y = cimagf(z);

    if (isinf(y))
        return CMPLXF(INFINITY, y);

    if (x == INFINITY)
        return CMPLXF(x, 0 * y);

    if (x == -INFINITY)
        return CMPLXF(y - y, copysignf(x, y));

    return _csqrt(x, y);
}
