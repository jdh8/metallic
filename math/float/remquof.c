/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../reinterpret.h"
#include <math.h>

float remquof(float numerator, float denominator, int quotient[static 1])
{
    if (__bitsf(fabsf(denominator)) < 0x7E000000)
        numerator = fmodf(numerator, 8 * denominator);

    float q = rintf(numerator / denominator);
    float r = numerator - denominator * (double)q;

    *quotient = q;

    return r ? r : copysignf(r, numerator);
}
