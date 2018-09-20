/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "cisf.h"
#include "prec/expm1f.h"
#include <complex.h>

float _Complex ccosf(float _Complex z)
{
    double _Complex circular = _cisf(z);

    double y = _expm1f(fabsf(cimagf(z)));
    double cosh = 0.5 * (y + 1) + 0.5 / (y + 1);
    double sinh = copysign(y * (0.5 + 0.5 / (y + 1)), -cimagf(z));

    return CMPLXF(cosh * (double)circular, sinh * cimag(circular));
}
