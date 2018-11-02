/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <stdint.h>

static unsigned __int128 _fixunsdfti(double x)
{
    if (x < 1)
        return 0;

    uint64_t high = 0x1p-64 * x;
    uint64_t low = x - 0x1p64 * high;

    return (unsigned __int128)high << 64 | low;
}
