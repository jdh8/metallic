/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../assert.h"
#include <stdint.h>

unsigned __int128 __multi3(unsigned __int128 a, unsigned __int128 b);

static void run2(unsigned __int128 a, unsigned __int128 b)
{
    unsigned __int128 c = __multi3(a, b);

    metallic_assert((uint64_t)a * (uint64_t)b == (uint64_t)c);
    metallic_assert(b * a == c);
    metallic_assert(-a * b == -c);
    metallic_assert(a * -b == -c);
    metallic_assert(-a * -b == c);
}

static void run3(unsigned __int128 a, unsigned __int128 b, unsigned __int128 c)
{
    metallic_assert(__multi3(a, b + c) == a * b + a * c);
    metallic_assert(a * __multi3(b, c) == a * b * c);
}

int main(void)
{
    for (unsigned __int128 a = 1; a >> 116 != 0xFFF; a *= 3)
        for (unsigned __int128 b = 1; b >> 116 != 0xFFF; b *= 5)
            run2(a, b);

    for (unsigned __int128 a = 1; a >> 120 != 0xFF; a *= 3)
        for (unsigned __int128 b = 1; b >> 120 != 0xFF; b *= 5)
            for (unsigned __int128 c = 1; c >> 120 != 0xFF; c *= 7)
                run3(a, b, c);
}

