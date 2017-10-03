/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <math.h>
#include <stdint.h>

float logbf(float x)
{
    x = fabsf(x);
    int32_t word = *(int32_t*)&x;

    if (word == 0)
        return -HUGE_VALF;

    if (word < 0x00800000)
        return -(118 + __builtin_clz(word));

    if (word < 0x7F800000)
        return (word >> 23) - 127;

    return x;
}
