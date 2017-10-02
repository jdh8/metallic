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

float cbrtf(float a)
{
    const double c3 = 0.333333333333333333;
    const uint64_t inf = 0x7FF0000000000000;
    const uint64_t magic = 0x2AA0000000000000; // *(uint64_t*)&(1.0) * 2 / 3

    double b = fabsf(a);
    uint64_t i = *(uint64_t*)&b;

    if (i >= inf)
       return a; 

    i = magic + i / 3;

    double x = *(double*)&i;

    x += c3 * (b / (x * x) - x);
    x *= 0.5 + 1.5 * b / (2*x*x*x + b);

    return copysignf(x, a);
}
