/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "digit.h"
#include "../../math/reinterpret.h"
#include <math.h>

static Scalar _nan(const char s[restrict static 1], char* end[restrict static 1], int tail)
{
    Bitset mantissa = 0;
    unsigned base = 10;
    *end = (char*)(s - 1);

    if (*s == '0') {
        if ((*++s | 32) == 'x') {
            base = 16;
            ++s;
        }
        else base = 8;
    }

    for (unsigned digit = _digit(*s); digit < base; digit = _digit(*++s))
        mantissa = mantissa * base + digit;

    if (*s == tail) {
        *end = (char*)(s + 1);
        const Bitset mask = (Bitset)-1 >> 1;
        const Bitset canonical = reinterpret(Bitset, (Scalar)NAN);
        return reinterpret(Scalar, canonical | (mantissa & mask));
    }

    return NAN;
}
