/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "common.h"
#include "../../../src/math/float/hypotf.c"
#include <assert.h>

static void run(float x, float y)
{
    double r = hypot(x, y);

    assert(approx(r, hypotf(x, y)));
    assert(approx(r, hypotf(y, x)));
    assert(approx(r, hypotf(-x, y)));
    assert(approx(r, hypotf(-y, x)));
    assert(approx(r, hypotf(-x, -y)));
    assert(approx(r, hypotf(-y, -x)));
    assert(approx(r, hypotf(x, -y)));
    assert(approx(r, hypotf(y, -x)));
}

int main(void)
{
    assert(hypotf(INFINITY, INFINITY) == INFINITY);
    assert(hypotf(-INFINITY, INFINITY) == INFINITY);
    assert(hypotf(-INFINITY, -INFINITY) == INFINITY);
    assert(hypotf(INFINITY, -INFINITY) == INFINITY);

    assert(hypotf(INFINITY, NAN) == INFINITY);
    assert(hypotf(-INFINITY, NAN) == INFINITY);
    assert(hypotf(NAN, INFINITY) == INFINITY);
    assert(hypotf(NAN, -INFINITY) == INFINITY);

    for (uint32_t j = 0; j < 0x7F800000; j += 0x000ABCDE) {
        float y = reinterpret(float, j);
        run(y, INFINITY);

        for (uint32_t i = 0; i < 0x7F800000; i += 0x00098765)
            run(reinterpret(float, i), y);
    }
}
