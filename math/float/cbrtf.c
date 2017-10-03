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

static int32_t _normalize(int32_t i)
{
    int shift = __builtin_clz(i) - 8;

    if (shift > 0)
        return (i << shift) - (shift << 23);

    return i;
}

static float _fast(float x)
{
    const int32_t magic = 0x2A555555; // 0x3F800000 * 2 / 3

    int32_t i = *(int32_t*)&x;

    if (i >= 0x7F800000)
       return x;

    i = magic + _normalize(i) / 3;

    return *(float*)&i;
}

float cbrtf(float a)
{
    float b = fabsf(a);
    double x = _fast(b);

    x += (1.0 / 3) * (b / (x * x) - x);
    x *= 0.5 + 1.5 * b / (2 * x * x * x + b);

    return copysignf(x, a);
}
