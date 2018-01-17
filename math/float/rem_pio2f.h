/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../reinterpret.h"
#include "../round.h"
#include <math.h>
#include <stddef.h>

uint64_t __2opi_64(ptrdiff_t);

inline int __rem_pio2f(float x, double y[static 1])
{
    const double pi_2[] = { 1.57079631090164184570, 1.58932547735281966916e-8 };
    const double _2_pi = 0.63661977236758134308;

    int32_t i = __bitsf(x);
    int32_t magnitude = i & 0x7FFFFFFF;

    if (magnitude < 0x4DC90FDB) {
        double q = __sint(_2_pi * x);
        *y = x - q * pi_2[0] - q * pi_2[1];
        return q;
    }

    if (magnitude >= 0x7F800000) {
        *y = x - x;
        return 0;
    }

    const double pi_2_65 = 8.51530395021638647334e-20;

    int32_t exp = magnitude >> 23;
    int32_t mantissa = (i & 0x007FFFFF) | 0x00800000;
    int32_t shift = __builtin_ctz(mantissa);

    uint64_t product = (mantissa >> shift) * __2opi_64(exp + shift - 90);
    int64_t r = product << 4;
    int q = (product >> 60) + (r < 0);

    *y = copysign(pi_2_65, x) * r;

    return i < 0 ? -q : q;
}
