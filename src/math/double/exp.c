/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2019 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "kernel/exp.h"
#include "kernel/ldexp.h"
#include <math.h>
#include <float.h>

double exp(double x)
{
    const double minimum = -745.1332191019412;
    const double maximum = 709.782712893385;

    const double log2e = 1.44269504088896340736;
    const double ln2[] = { 0x1.62E42FEFA4p-1, -0x1.8432A1B0E2634p-43 };

    if (x < minimum)
        return 0;

    if (x > maximum)
        return maximum * DBL_MAX;

    double n = rint(x * log2e);
    double a = x - n * ln2[0];
    double b = n * -ln2[1];

    return _kernel_ldexp(_kernel_expb(a, b), n);
}