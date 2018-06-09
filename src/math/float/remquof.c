/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017, 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../reinterpret.h"
#include "../round.h"
#include <math.h>

float remquof(float numerator, float denominator, int quotient[static 1])
{
    float r = fmodf(numerator, 8 * denominator);
    float q = __rintf(r / denominator);

    if ((*quotient = q))
        r -= denominator * (double)q;

    return r ? r : copysignf(r, numerator);
}
