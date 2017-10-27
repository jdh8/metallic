/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
static double quadrant_sinf(double x)
{
    const double c[] = {
        9.9999999692634277740e-1,
       -1.6666650699202422675e-1,
        8.3320368751598070196e-3,
       -1.9504022000675618505e-4
    };

    double xx = x * x;

    return x * (c[0] + c[1] * xx + (c[2] + c[3] * xx) * (xx * xx));
}

static double quadrant_cosf(double x)
{
    const double c[] = {
        9.9999997242332292107e-1,
       -4.9999856695848847717e-1,
        4.1655026884251524438e-2,
       -1.3585908510113298585e-3
    };

    x *= x;

    return c[0] + c[1] * x + (c[2] + c[3] * x) * (x * x);
}
