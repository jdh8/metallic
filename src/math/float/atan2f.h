/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "kernel/atanf.h"
#include <math.h>

static double _atan2f(double y, double x)
{
    const double pi = 3.14159265358979323846;

    double a = fabs(x);
    double b = fabs(y);

    if (a < b)
        return copysign(pi / 2, y) - _kernel_atanf(x / y);

    double acute = a == b ? b ? copysign(pi / 4, y) : y : _kernel_atanf(y / a);

    return signbit(x) ? copysign(pi, y) - acute : acute;
}
