/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "common.h"
#include "../../../src/math/float/atan2f.c"
#include <assert.h>

static void run(float y, float x)
{
    double first = atan2(y, x);
    double second = atan2(y, -x);

    assert(approx(first, atan2f(y, x)));
    assert(approx(second, atan2f(y, -x)));
    assert(approx(-first, atan2f(-y, x)));
    assert(approx(-second, atan2f(-y, -x)));
}

int main(void)
{
    assert(atan2f(INFINITY, INFINITY) == (float)atan(1));

    for (uint32_t j = 0; j < 0x7F800000; j += 0x000ABCDE)
        for (uint32_t i = 0; i < 0x7F800000; i += 0x00098765)
            run(reinterpret(float, j), reinterpret(float, i));
}
