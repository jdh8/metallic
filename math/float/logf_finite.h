/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "atanhf_pade.h"
#include "logf_reduce.h"

static double logf_finite(double x)
{
    const double ln2 = 0.6931471805599453094;
    int exponent;

    x = logf_reduce(x, &exponent);

    return 2 * atanhf_pade((x - 1) / (x + 1)) + exponent * ln2;
}
