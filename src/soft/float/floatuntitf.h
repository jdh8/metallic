/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../integer/clzti2.h"
#include "../../math/reinterpret.h"
#include <float.h>
#include <stdint.h>

static long double _floatuntitf(unsigned __int128 a)
{
    const unsigned __int128 mask = (unsigned __int128)-1 >> 16;

    if (!a)
        return 0;

    int space = _clzti2(a);
    unsigned __int128 significand = a << space >> 15 & mask;
    unsigned __int128 exp = 0x3FFF + 127 - space;

    return reinterpret(long double, significand | exp << 112);
}
