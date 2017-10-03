/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "atanf/octant.h"
#include <math.h>

float acosf(float x)
{
    double c = x;
    double s = sqrt(1 - c * c);

    return 2 * atanf_octant((1 - c) / s);
}
