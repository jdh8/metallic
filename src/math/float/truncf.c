/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../reinterpret.h"
#include <stdint.h>

static float _truncf(float x)
{
    const int32_t mask = 0xFF800000;
    int32_t bits = reinterpret(int32_t, x);
    int32_t magnitude = bits & 0x7FFFFFFF;

    if (magnitude < 0x3F800000) /* 1 */
        return reinterpret(float, bits & 0x80000000);

    if (magnitude < 0x4B000000) /* 0x1p23 */
        return reinterpret(float, mask >> ((magnitude >> 23) - 127) & bits);

    return x;
}

#if defined(__wasm__) || defined(__AVX__) || defined(__SSE4_1__)
#define TRUNCF(x) __builtin_truncf(x)
#else
#define TRUNCF(x) _truncf(x)
#endif

float truncf(float x)
{
    (void)_truncf;
    return TRUNCF(x);
}
