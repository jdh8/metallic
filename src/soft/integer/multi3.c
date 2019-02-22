/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "kernel/umuldi.h"
#include <stdint.h>

unsigned __int128 __multi3(unsigned __int128 a, unsigned __int128 b)
{
    uint64_t a1 = a >> 64;
    uint64_t b1 = b >> 64;
    uint64_t a0 = a;
    uint64_t b0 = b;

    return ((unsigned __int128)(a1 * b0 + a0 * b1) << 64) + _umuldi(a0, b0);
}
