/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "cisf.h"
#include "prec/expf.h"
#include <complex.h>

float _Complex csinhf(float _Complex z)
{
    double _Complex circular = _cisf(cimagf(z));

    double exp = _expf(z);
    double cosh = 0.5 * exp + 0.5 / exp;
    double sinh = 0.5 * exp - 0.5 / exp;

    return CMPLXF(sinh * (double)circular, cosh * cimag(circular));
}
