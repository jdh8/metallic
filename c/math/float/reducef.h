/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "frexpf.h"

static float reducef(float x, int* exp)
{
    x = __frexpf(x, exp);

    uint32_t word = *(uint32_t*)&x;

    if (word << 1 <= 0x7E6A09E6) {
        word |= 0x00800000;
        --*exp;
    }

    return *(float*)&word;
}
