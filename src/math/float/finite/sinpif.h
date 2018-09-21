/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <math.h>

static double _kernel_sinpif(double x)
{
    const double c[] = {
        3.14159264393361429, /* No, this is not pi */
       -5.16770782913382771,
        2.54976729815574562,
       -0.589078655630501296
    };

    double xx = x * x;

    return x * (c[0] + c[1] * xx + (c[2] + c[3] * xx) * (xx * xx));
}

static double _kernel_cospif(double x)
{
    const double c[] = {
        0.999999972423322921,
       -4.93478805699187024,
        4.05757830579187565,
       -1.30613456265256899
    };

    x *= x;

    return c[0] + c[1] * x + (c[2] + c[3] * x) * (x * x);
}

static double _sinpif(float x)
{
    x -= 2 * rintf(x / 2);

    float q = rintf(2 * x);
    float r = x - q / 2;

    switch (3u & (int) q) {
        case 0:
            return _kernel_sinpif(r);
        case 1:
            return _kernel_cospif(r);
        case 2:
            return _kernel_sinpif(-r);
        case 3:
            return -_kernel_cospif(r);
    }

    __builtin_unreachable();
}
