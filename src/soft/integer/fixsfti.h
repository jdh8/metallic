/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <math.h>
#include <stdint.h>

static __int128 _fixsfti(float x)
{
    float r = fabsf(x);
    uint64_t high = 0x1p-64f * r;
    uint64_t low = r - 0x1p64f * high;
    __int128 abs = (__int128)high << 64 | low;

    return signbit(x) ? -abs : abs;
}
