/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
/*!\file
 * \brief Complex inverse hyperbolic functions in the first quadrant
 */
#include "csqrt.h"
#include "log1pf.h"
#include "../kernel/atanf.h"
#include "../kernel/atanhf.h"
#include "../../reinterpret.h"
#include <stdint.h>

static double _real_clogf(double x, double y)
{
    const double ln2 = 0.69314718055994530942;

    int64_t i = reinterpret(int64_t, x * x + y * y);
    int64_t exponent = (i - 0x3FE6A09E667F3BCD) >> 52;
    double z = reinterpret(double, i - (exponent << 52));

    return _kernel_atanhf((z - 1) / (z + 1)) + ln2 / 2 * exponent;
}

static double _carg(double x, double y)
{
    const double pi = 3.14159265358979323846;

    return x < y ? pi / 2 - _kernel_atanf(x / y) : _kernel_atanf(y / x);
}

/*!
 * \brief Complex inverse hyperbolic functions in the first quadrant
 *
 * This function computes inverse hyperbolic sine or cosine in the first
 * quadrant of the complex plane.  It depends on the applied argument function
 * whether to compute asinh or acosh.
 *
 * \param x - Real part for asinh, imaginary part for acosh
 * \param y - Imaginary part for asinh, real part for acosh
 * \param arg - carg for asinh, atan2 for acosh
 * \return  Approximate \f$ \operatorname{asinh} \left( x + iy \right) \f$
 *          or \f$ \operatorname{acosh} \left( y + ix \right) \f$.
 */
static double _Complex _cashf(double x, double y, double arg(double, double))
{
    double xx = x * x;
    double yy = y * y;

    if (x < 0.5) {
        if (y < 1) {
            double s = xx * (2 + xx + 2 * yy);
            double t = 1 - yy;
            double u = sqrt(s + t * t) + t;
            double dx = sqrt(0.5 * (u + xx));
            double dy = x * y / dx;
            double re = 0.5 * _log1pf(xx + s / u + 2 * (x * dx + y * dy));
            double im = arg(x + dx, y + dy);

            return CMPLX(re, im);
        }
        if (y == 1) {
            double r = x * sqrt(4 + xx);
            double dx = sqrt(0.5 * (r + xx));
            double dy = sqrt(0.5 * (r - xx));
            double re = 0.5 * _log1pf(xx + r + 2 * (x * dx + dy));
            double im = arg(x + dx, 1 + dy);

            return CMPLX(re, im);
        }
    }

    double _Complex z = CMPLX(x, y) + _csqrt(xx - yy + 1, 2 * x * y);
    double re = z;
    double im = cimag(z);

    return CMPLX(_real_clogf(re, im), arg(re, im));
}
