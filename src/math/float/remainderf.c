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

float remainderf(float numerator, float denominator)
{
    float r = fmodf(numerator, 2 * denominator);
    float q = __rintf(r / denominator);

    if (q)
        r -= denominator * (double)q;

    return r ? r : copysignf(r, numerator);
}
