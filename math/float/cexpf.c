/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../reinterpret.h"
#include "kernel/sincosf.h"
#include "prec/expf.h"
#include <complex.h>
#include <math.h>
#include <stdint.h>

static double _Complex _cisf(float x)
{
    const double pi_2 = 1.57079632679489662;
    const float _2_pi = 0.6366197723676;
    const uint_least32_t thresh = 0x4F000000; /* 2 ** 31 */

    float q = nearbyintf(x * _2_pi);
    double r = x - pi_2 * q;

    if ((uint_least32_t)__bitsf(q) << 1 < thresh << 1) {
        switch (3 & (unsigned)(int_least32_t)q) {
            case 1:
                return CMPLX(__kernel_sinf(-r), __kernel_cosf(r));
            case 2:
                return CMPLX(-__kernel_cosf(r), __kernel_sinf(-r));
            case 3:
                return CMPLX(__kernel_sinf(r), -__kernel_cosf(r));
        }
    }

    return CMPLX(__kernel_cosf(r), __kernel_sinf(r));
}

float _Complex cexpf(float _Complex z)
{
    return __prec_expf(z) * _cisf(cimagf(z));
}
