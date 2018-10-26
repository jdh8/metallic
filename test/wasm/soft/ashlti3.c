/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../assert.h"

static void run(unsigned __int128 x)
{
    unsigned __int128 y = x;

    for (int shift = 0; shift < 128; ++shift) {
        metallic_assert(x << shift == y);
        y <<= 1;
    }
}

int main(void)
{
    for (unsigned __int128 x = 1; x; x *= 2)
        run(x);

    for (unsigned __int128 x = 1; x >> 104 != 0x313370; x *= 3)
        run(x);
}

