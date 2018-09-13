/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "normalize.h"
#include "../reinterpret.h"
#include <float.h>
#include <math.h>
#include <stdint.h>

double SCALBN(double x, Integer exp)
{
    int64_t i = reinterpret(int64_t, fabs(x));

    if (i == 0 || i >= 0x7FF0000000000000)
        return x;

    i = _normalize(i);

    int64_t biased = exp + (i >> (DBL_MANT_DIG - 1));

    if (biased >= 0x7FF || biased < -DBL_MANT_DIG)
        return x * (exp < 0 ? 0 : HUGE_VALF);

    i &= 0x000FFFFFFFFFFFFF;

    if (biased > 0)
        i |= biased << (DBL_MANT_DIG - 1);
    else
        i = (i | 0x0010000000000000) >> (1 - biased);

    return copysign(reinterpret(double, i), x);
}
