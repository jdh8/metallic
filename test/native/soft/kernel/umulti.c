/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2019 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "src/soft/integer/kernel/umulti.h"
#include <assert.h>

static uint64_t sum(unsigned __int128 a)
{
    uint32_t a0 = a;
    uint32_t a1 = a >> 32;
    uint32_t a2 = a >> 64;
    uint32_t a3 = a >> 96;

    return (uint64_t)a0 + a1 + a2 + a3;
}

int main(void)
{
    unsigned __int128 a = 0;
    unsigned __int128 b = 0;

    for (long i = 0; i < 1L << 26; ++i) {
        struct u256 c = _umulti(a, b);

        assert(c.low == a * b);
        assert((sum(c.high) + sum(c.low)) % 0xFFFFFFFF == (sum(a) % 0xFFFFFFFF) * (sum(b) % 0xFFFFFFFF) % 0xFFFFFFFF);

        a = a + 1993 + (a >> 3);
        b = b + 2019 + (b >> 4);
    }
}

