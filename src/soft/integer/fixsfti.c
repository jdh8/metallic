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

__int128 __fixsfti(float x)
{
    float high = floorf(0x1p-64f * x);
    uint64_t low = x - 0x1p64f * high;

    return (__int128)(int64_t)high << 64 | low;
}
