/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "kernel/erfcf.h"
#include <math.h>

static double _kernel(double x)
{
    const double c[] = {
        1.128379067473277685,
       -0.3761122431043507220,
        0.1125234763068763124,
       -0.02461177835483573533
    };

    double xx = x * x;

    return x * (c[0] + c[1] * xx + (c[2] + c[3] * xx) * (xx * xx));
}

static float _right(float x)
{
    return x > 0.4769362762044699f ? 1 - _kernel_erfcf(x) : _kernel(x);
}

float erff(float x)
{
    return copysignf(_right(fabsf(x)), x);
}
