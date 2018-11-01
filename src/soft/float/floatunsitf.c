/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../../math/reinterpret.h"
#include <stdint.h>

long double __floatunsitf(uint32_t a)
{
    if (!a)
        return 0;

    uint64_t high = (reinterpret(uint64_t, (double)a) >> 4) + 0x3C00000000000000;

    return reinterpret(long double, (unsigned __int128)high << 64);
}
