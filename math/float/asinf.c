/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "atanf.h"
#include <math.h>

float asinf(float x)
{
    double s = fabsf(x);
    double c = sqrt(1 - s * s);

    return copysignf(2 * octant_atanf(s / (1 + c)), x);
}
