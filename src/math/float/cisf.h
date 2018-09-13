/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017, 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "kernel/sincosf.h"
#include <complex.h>

static double _Complex _cisf(float t)
{
    double r;
    unsigned q = __rem_pio2f(t, &r);
    double x = _kernel_cosf(r);
    double y = _kernel_sinf(r);

    switch (q & 3) {
        case 1:
            return CMPLX(-y, x);
        case 2:
            return CMPLX(-x, -y);
        case 3:
            return CMPLX(y, -x);
    }

    return CMPLX(x, y);
}
