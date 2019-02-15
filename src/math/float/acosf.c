/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017, 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <math.h>

/* Arccosine restricted to [0, 1]
 *
 * If x > 1, this function returns NaN to indicate complex result.
 * If x < 0, the result is inaccurate.
 */
static double _kernel(double x)
{
    const double c[] = {
        1.5707963049952700155,
       -2.1459880383414681170e-1,
        8.8979049893610385888e-2,
       -5.0174715211875860817e-2,
        3.0893053200289071461e-2,
       -1.7089810818777579223e-2,
        6.6712932693206083568e-3,
       -1.2628309202213843948e-3
    };

    double xx = x * x;

    return sqrt(1 - x) * (c[0] + c[1] * x + (c[2] + c[3] * x) * xx
        + (c[4] + c[5] * x + (c[6] + c[7] * x) * xx) * (xx * xx));
}

float acosf(float x)
{
    const double pi = 3.14159265358979323846;
    double y = _kernel(fabsf(x));

    return x > 0 ? y : pi - y;
}
