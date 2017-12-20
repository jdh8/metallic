/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <complex.h>
#include <math.h>

float _Complex cprojf(float _Complex z)
{
    if (isinf((float)z) || isinf(cimagf(z)))
        return CMPLXF(HUGE_VALF, copysignf(0, cimagf(z)));

    return z;
}
