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

float atanf(float x)
{
    const double pi_2 = 1.5707963267948966192;

    if (fabsf(x) > 1)
        return pi_2 - atanf_octant(1.0 / x);
    else
        return atanf_octant(x);
}
