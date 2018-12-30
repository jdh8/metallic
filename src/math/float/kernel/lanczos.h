/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
static double _lanczos_series(double z)
{
    const double p[] = {
        2.5066282972608683788,
        93.724008703110370647,
       -85.030278048785721233,
        15.313090747320992728,
       -0.2376573970640603182
    };

    return p[4] / (z + 4) + p[3] / (z + 3) + (p[2] / (z + 2) + p[1] / (z + 1)) + p[0];
}

static const double _lanczos_g = 4.3644453082153116114;
