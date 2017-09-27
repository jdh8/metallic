/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <limits.h>
#include <stdint.h>

static float engineeringf(float x, int* exp)
{
    uint32_t word = *(uint32_t*)&x;
    uint32_t wabs = word << 1;

    if (wabs >= 0xFF000000 || wabs == 0) {
        *exp = 0;
        return x;
    }

    int shift = __builtin_clz(wabs) + 1;

    if (shift > 8) {
        *exp = -(118 + shift);
        word <<= (shift - 8);
    }
    else {
        *exp = (wabs >> 23) - 127;
    }

    word = (word & 0x807FFFFF) | 0x3F800000;

    if (word << 1 > 0x7F6A09E6) { /* x > sqrt(2) */
        ++*exp;
        word -= 0x00800000;
    }
    
    return *(float*)&word;
}
