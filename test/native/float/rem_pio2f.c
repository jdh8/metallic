/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "common.h"
#include "../../../src/math/rem_pio2.c"
#include <assert.h>

static double quadrant(float x, double y[static 1])
{
    switch (__rem_pio2f(x, y) & 3u) {
        case 0:
            return asin(sin(x));
        case 1:
            return asin(-cos(x));
        case 2:          
            return asin(-sin(x));
        case 3:
            return asin(cos(x));
    }

    __builtin_unreachable();
}

int main(void)
{
    for (int32_t i = 0; i < 0x7F800000; i += 75) {
        float x = reinterpret(float, i);
        double y1;
        double y0 = quadrant(x, &y1);

        verify(approx(y0, y1), x);
    }
}
