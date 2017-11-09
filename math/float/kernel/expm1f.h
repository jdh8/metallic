/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#ifndef METALLIC_KERNEL_EXPM1F_H
#define METALLIC_KERNEL_EXPM1F_H
/*!
 * \brief Kernel of expm1f
 *
 * This computes exponential functon in a domain of \f$ [-\frac13,
 * \frac13] \f$.  The result is guaranteed to be faithfully rounded in
 * \c float, whose relative error is controlled within 2.819428e-9.
 *
 * If \a x is outside of \f$ [-\frac13, \frac13 ] \f$,
 * the result is inaccurate.
 *
 * \param x - The argument in \f$ [-\frac13, \frac13 ] \f$
 * \return  Approximate \f$ \exp x - 1 \f$ as precise as \c float.
 */
inline double __kernel_expm1f(double x)
{
    const double c[] = {
        2.000000002806476762,
        1.666662104301108501e-1,
       -2.766800136235738314e-3
    };

    double xx = x * x;
    double xcothx_2 = c[0] + c[1] * xx + c[2] * (xx * xx);

    return 2 * x / (xcothx_2 - x);
}
#endif
