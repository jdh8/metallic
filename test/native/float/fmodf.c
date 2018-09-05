/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "common.h"
#include "../../../src/math/float/fmodf.c"
#include <assert.h>
#include <stdint.h>

static void run(float numerator, float denominator)
{
    float r0 = fmodf(numerator, denominator);

    assert(identical(r0, _fmodf(numerator, denominator)));
    assert(identical(r0, _fmodf(numerator, -denominator)));
    assert(identical(-r0, _fmodf(-numerator, denominator)));
    assert(identical(-r0, _fmodf(-numerator, -denominator)));
}

int main(void)
{
    assert(isnan(_fmodf(INFINITY, INFINITY)));
    assert(isnan(_fmodf(-INFINITY, INFINITY)));
    assert(isnan(_fmodf(-INFINITY, -INFINITY)));
    assert(isnan(_fmodf(INFINITY, -INFINITY)));

    assert(isnan(_fmodf(NAN, INFINITY)));
    assert(isnan(_fmodf(NAN, -INFINITY)));

    assert(isnan(_fmodf(INFINITY, NAN)));
    assert(isnan(_fmodf(-INFINITY, NAN)));

    assert(isnan(_fmodf(NAN, NAN)));

    for (uint32_t j = 6; j < 0x7F800000; j += 0x000ABCDE)
        for (uint32_t i = 0; i < 0x7F800000; i += 0x00098765)
            run(reinterpret(float, i), reinterpret(float, j));
}
