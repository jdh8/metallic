/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <math.h>

static double _rint(double x)
{
    const double rectifier = 0x1p52;
    double r = fabs(x);

    if (r < rectifier) {
        double y = r + rectifier;
        return copysign(y - rectifier, x);
    }

    return x;
}

#if defined(__wasm__) || defined(__AVX__) || defined(__SSE4_1__)
#define RINT(x) __builtin_rint(x)
#else
#define RINT(x) _rint(x)
#endif

double rint(double x)
{
    (void)_rint;
    return RINT(x);
}

#ifdef __wasm__
double nearbyint(double) __attribute__((alias("rint")));
#endif
