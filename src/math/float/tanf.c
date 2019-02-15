/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
/*!\file
 * \brief Tangent function
 */
int __rem_pio2f(float x, double y[static 1]);

/*!
 * \brief Kernel of tangent
 *
 * This computes the limit of \f$ \sqrt x \cot \sqrt x \f$
 * for \a x in \f$ \left[ 0, \left( \frac\pi4 \right)^2 \right] \f$.
 * The result is guaranteed to be faithfully rounded in \c float,
 * whose absolute error is controlled within 4.520299e-9.
 *
 * If \a x is outside of \f$ \left[ 0, \left( \frac\pi4 \right)^2 \right] \f$,
 * the result is inaccurate.
 *
 * \param x - The argument in \f$ \left[ 0, \left( \frac\pi4 \right)^2 \right] \f$.
 * \return  Approximate \f$ \sqrt x \cot \sqrt x \f$ as precise as \c float.
 */
static double _kernel(double x)
{
    const double c[] = {
        9.9999999547970088679e-1,
       -3.3333297156778390385e-1,
       -2.2226847402057085257e-2,
       -2.0958255714778903209e-3,
       -2.4838647503256150943e-4
    };

    double xx = x * x;

    return c[0] + c[1] * x + (c[2] + c[3] * x) * xx + c[4] * (xx * xx);
}

float tanf(float x)
{
    double y;
    unsigned q = __rem_pio2f(x, &y);
    double ycoty = _kernel(y * y);

    return q & 1 ? ycoty / -y : y / ycoty;
}
