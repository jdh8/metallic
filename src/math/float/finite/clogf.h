
/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../atan2f.h"
#include "loghypotf.h"
#include <complex.h>

static double _Complex _clogf(double _Complex z)
{
    double x = z;
    double y = cimag(z);

    return CMPLX(_loghypotf(x, y), _atan2f(y, x));
}
