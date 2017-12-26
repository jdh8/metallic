/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <math.h>
#include <limits.h>

static long _invalid(float x)
{
//  feraiseexcept(FE_INVALID);
    return signbit(x) ? LONG_MIN : LONG_MAX;
}

long lrintf(float x)
{
    x = rintf(x);

    if (x >= LONG_MIN && x <= LONG_MAX)
        return x;
    else
        return _invalid(x);
}
