/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../../math/reinterpret.h"
#include "../../math/double/normalize.h"
#include <math.h>
#include <stdint.h>

static __int128 _magnitude(int64_t i)
{
    if (i >= 0x7FF0000000000000)
        return (__int128)0x78 << 120 | (__int128)i << 60;

    return i ? ((__int128)0x3C << 120) + ((__int128)_normalize(i) << 60) : 0;
}

long double __extenddftf2(double x)
{
    int64_t i = reinterpret(int64_t, fabs(x));

    return copysignl(reinterpret(long double, _magnitude(i)), x);
}
