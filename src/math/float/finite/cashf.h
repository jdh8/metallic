/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "csqrt.h"
#include "log1pf.h"
#include "loghypotf.h"
/*!
 * \brief Kernel of complex inverse hyperbolic functions
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

    return CMPLX(_loghypotf(re, im), arg(re, im));
}
