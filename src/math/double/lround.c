/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "round.h"
#include <math.h>
#include <limits.h>

long lround(double x)
{
    double y = LONG_MAX < 0x1p52 ? x + copysign(0.5, x) : _round(x);

    return y >= LONG_MAX ? LONG_MAX : y > LONG_MIN ? (long)y : LONG_MIN;
}
