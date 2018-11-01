/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../../math/reinterpret.h"
#include <math.h>
#include <stdint.h>

uint64_t _magnitude(uint64_t i)
{
    if (i >= 0x7FF0000000000000)
        return 0x7800000000000000 | i >> 4;

    return i ? 0x3C00000000000000 + (i >> 4) : 0;
}

long double __extendsftf2(float x)
{
    uint64_t i = reinterpret(uint64_t, (double)x);
    uint64_t high = _magnitude(i & INT64_MAX) | (i & 0x8000000000000000);

    return reinterpret(long double, (unsigned __int128)high << 64);
}
