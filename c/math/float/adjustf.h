/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "frexpf.h"

static float adjustf(float bottom, float x, int* exp)
{
    x = __frexpf(x, exp);

    uint32_t a = *(uint32_t*)&x;
    uint32_t b = *(uint32_t*)&bottom;

    if ((a & 0x7FFFFFFF) <= b) {
        a |= 0x00800000;
        --*exp;
    }

    return *(float*)&a;
}
