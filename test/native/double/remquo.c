/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../../../src/math/double/remquo.c"
#include "unary.h"
#include <assert.h>

static _Bool coterminal(unsigned q0, unsigned q1)
{
    return !((q0 ^ q1) & 7);
}

static void run(double numerator, double denominator)
{
    int q0, q1;
    double r0 = remquo(numerator, denominator, &q0);

    assert(identical(r0, _remquo(numerator, denominator, &q1)) && coterminal(q0, q1));
    assert(identical(r0, _remquo(numerator, -denominator, &q1)) && coterminal(q0, q1));
    assert(identical(-r0, _remquo(-numerator, denominator, &q1)) && coterminal(-q0, q1));
    assert(identical(-r0, _remquo(-numerator, -denominator, &q1)) && coterminal(-q0, q1));
}

int main(void)
{
    const uint64_t inf = 0x7FF0000000000000;
    const uint64_t delta = 0x000C04410EAAA8FF;
    const uint64_t model = 0x42B58853B5377245;

    for (uint64_t i = 0; i < inf; i += delta) {
        for (int shift = 0; shift < 63; ++shift)
            run(reinterpret(double, i), reinterpret(double, model >> shift));
        for (uint64_t mask = 2; mask < 0x8000000000000000; mask <<= 1)
            run(reinterpret(double, i), reinterpret(double, (model & -mask) | mask));
    }
}
