/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../kernel/atanhf.h"
#include "../../reinterpret.h"
#include <float.h>
#include <stdint.h>

static double __prec_log1pf(double x)
{
    const double ln2 = 0.69314718055994530942;

    int64_t i = reinterpret(int64_t, 1 + x);
    int64_t exponent = (i - 0x3FE6A09E667F3BCD) >> (DBL_MANT_DIG - 1);
    double y = reinterpret(double, i - (exponent << (DBL_MANT_DIG - 1)));

    if (exponent)
        return 2 * __kernel_atanhf((y - 1) / (y + 1)) + exponent * ln2;
    else
        return 2 * __kernel_atanhf(x / (2 + x));
}
