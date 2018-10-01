/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../../../src/math/float/remquof.c"
#include "unary.h"
#include <assert.h>

static _Bool coterminal(unsigned q0, unsigned q1)
{
    return !((q0 ^ q1) & 7);
}

static void run(float numerator, float denominator)
{
    int q0, q1;
    float r0 = remquof(numerator, denominator, &q0);

    assert(identical(r0, remquof(numerator, denominator, &q1)) && coterminal(q0, q1));
    assert(identical(r0, remquof(numerator, -denominator, &q1)) && coterminal(q0, q1));
    assert(identical(-r0, remquof(-numerator, denominator, &q1)) && coterminal(-q0, q1));
    assert(identical(-r0, remquof(-numerator, -denominator, &q1)) && coterminal(-q0, q1));
}

int main(void)
{
    for (uint32_t j = 594; j < 0x7F800000; j += 0x002ABCDE)
        for (uint32_t i = 0; i < 0x7F800000; i += 0x00234567)
            run(reinterpret(float, i), reinterpret(float, j));
}
