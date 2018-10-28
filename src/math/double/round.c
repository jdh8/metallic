/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <math.h>

static double _round(double x)
{
    double r = fabs(x);
    double i = trunc(r);

    return copysign(i + (r - i >= 0.5), x);
}

#ifdef _METALLIC
double round(double x) { return _round(x); }
#endif
