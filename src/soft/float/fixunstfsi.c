/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../../math/reinterpret.h"
#include <stdint.h>

uint32_t __fixunstfsi(long double x)
{
    int64_t high = reinterpret(__int128, x) >> 64;

    if (high < 0x3FFF000000000000)
        return 0;

    return reinterpret(double, (high - 0x3C00000000000000) << 4);
}
