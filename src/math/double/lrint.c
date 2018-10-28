/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <math.h>
#include <limits.h>

long lrint(double x)
{
    x = rint(x);

    return x >= LONG_MAX ? LONG_MAX : x > LONG_MIN ? (long)x : LONG_MIN;
}
