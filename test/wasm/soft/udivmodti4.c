/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../../../src/soft/integer/udivmodti4.h"
#include "../assert.h"

static void run(unsigned __int128 a, unsigned __int128 b)
{
    unsigned __int128 r;
    unsigned __int128 q = _udivmodti4(a, b, &r);

    metallic_assert(a == b * q + r);
}

int main(void)
{
    for (unsigned __int128 a = 1; a >> 116 != 0xFFF; a *= 3)
        for (unsigned __int128 b = 1; b >> 116 != 0xFFF; b *= 5)
            run(a, b);
}


