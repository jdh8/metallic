/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017, 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "kernel/atanf.h"
#include <math.h>

float atanf(float x)
{
    const double pi_2 = 1.57079632679489661923;

    return fabsf(x) > 1 ? copysign(pi_2, x) - __kernel_atanf(1.0 / x) : __kernel_atanf(x);
}
