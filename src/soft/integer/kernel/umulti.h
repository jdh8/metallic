/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2019 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "umuldi.h"

struct u256
{
    unsigned __int128 low;
    unsigned __int128 high;
};

static struct u256 _umulti(unsigned __int128 a, unsigned __int128 b)
{
    uint64_t a1 = a >> 64;
    uint64_t b1 = b >> 64;
    uint64_t a0 = a;
    uint64_t b0 = b;
    
    unsigned __int128 a1b1 = _umuldi(a1, b1);
    unsigned __int128 a1b0 = _umuldi(a1, b0);
    unsigned __int128 a0b1 = _umuldi(a0, b1);
    unsigned __int128 a0b0 = _umuldi(a0, b0);

    unsigned __int128 mid = (a0b0 >> 64) + (uint64_t)a1b0 + (uint64_t)a0b1;

    return (struct u256) {
        mid << 64 | (uint64_t)a0b0,
        (mid >> 64) + (a1b0 >> 64) + (a0b1 >> 64) + a1b1
    };
}
