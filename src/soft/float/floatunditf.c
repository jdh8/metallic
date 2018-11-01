/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../../math/reinterpret.h"
#include <stdint.h>

long double __floatunditf(uint64_t a)
{
    if (!a)
        return 0;

    int space = __builtin_clzll(a);
    unsigned __int128 significand = (unsigned __int128)(a << space & INT64_MAX) << 49;
    unsigned __int128 exp = 0x3FFF + 63 - space;

    return reinterpret(long double, significand | exp << 112);
}
