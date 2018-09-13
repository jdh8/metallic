/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017, 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <math.h>
#include <limits.h>

long lroundf(float x)
{
    double y = x + copysign(0.5, x);

    return y >= LONG_MAX ? LONG_MAX : y > LONG_MIN ? (long)y : LONG_MIN;
}
