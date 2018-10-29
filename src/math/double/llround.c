/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "round.h"
#include <limits.h>

long long llround(double x)
{
    double y = _round(x);

    return y >= LLONG_MAX ? LLONG_MAX : y > LLONG_MIN ? (long long)y : LLONG_MIN;
}
