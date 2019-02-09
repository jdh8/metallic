/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017, 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "kernel/exp2m1f.h"
#include "../double/shift.h"
#include <math.h>
#include <float.h>

float exp2f(float x)
{
    if (x < -150)
        return 0;

    if (x > 128)
        return 128 * FLT_MAX;

    float n = rintf(x);
    double y = 1 + _kernel_exp2m1f(x - n);

    return _shift(y, n);
}
