/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <math.h>

static double sinpif_octant(double x)
{
    const double c[] = {
        3.14159265358979324,  /* pi         */
        5.16771278004997003,  /* pi**3 / 3! */
        2.55016403987734544,  /* pi**5 / 5! */
        0.599264529320792077, /* pi**7 / 7! */
        0.0821458866111282288 /* pi**9 / 9! */
    };

    double xx = x * x;

    return x * (c[0] - xx * (c[1] - xx * (c[2] - xx * (c[3] - xx * c[4]))));
}

static double cospif_octant(double x)
{
    const double c[] = {
        1,
        4.93480220054467931, /* pi**2 / 2! */
        4.05871212641676822, /* pi**4 / 4! */
        1.33526276885458950, /* pi**6 / 6! */
        0.235330630358893205 /* pi**8 / 8! */
    };

    x *= x;

    return c[0] - x * (c[1] - x * (c[2] - x * (c[3] - x * c[4])));
}

static double sinpif(float x)
{
    x -= 2 * nearbyintf(x / 2);

    float q = nearbyintf(2 * x);
    float r = x - q / 2;

    switch (3u & (int) q) {
        case 0:
            return sinpif_octant(r);
        case 1:
            return cospif_octant(r);
        case 2:
            return sinpif_octant(-r);
        case 3:
            return -cospif_octant(r);
    }

    __builtin_unreachable();
}
