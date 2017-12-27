/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../round.h"
#include <math.h>

float remquof(float numerator, float denominator, int* quotient)
{
    double a = numerator;
    double b = denominator;
    double q = __rint(a / b);

    *quotient = q - 8 * trunc(q / 8);

    return a - b * q;
}
