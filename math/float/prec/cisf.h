/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../kernel/sincosf.h"
#include "../../reinterpret.h"
#include <complex.h>
#include <math.h>
#include <stdint.h>

inline double _Complex __prec_cisf(double t)
{
    const double pi_2 = 1.57079632679489662;
    const double _2_pi = 0.636619772367581343;
    const uint_least64_t thresh = 0x43E0000000000000; /* 2 ** 63 */

    double q = nearbyint(t * _2_pi);
    double r = t - pi_2 * q;
    double x = __kernel_cosf(r);
    double y = __kernel_sinf(r);

    if ((uint_least64_t)__bits(q) << 1 < thresh << 1) {
        switch (3 & (unsigned)(int_least64_t)q) {
            case 1:
                return CMPLX(-y, x);
            case 2:
                return CMPLX(-x, -y);
            case 3:
                return CMPLX(y, -x);
        }
    }

    return CMPLX(x, y);
}
