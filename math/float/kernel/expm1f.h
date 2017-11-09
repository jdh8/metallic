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
 * This computes exponential functon in a domain of \f$ [-0.5,
 * 0.5] \f$.  The result is guaranteed to be faithfully rounded in
 * \c float, whose relative error is controlled within 3.189370e-8.
 *
 * If \a x is outside of \f$ [-0.5, 0.5] \f$,
 * the result is inaccurate.
 *
 * \param x - The argument in \f$ [-0.5, 0.5] \f$
 * \return  Approximate \f$ \exp x - 1 \f$ as precise as \c float.
 */
inline double __kernel_expm1f(double x)
{
    const double c[] = {
        2.000000031893695369,
        1.666643655421171822e-1,
       -2.753162177947982655e-3
    };

    double xx = x * x;
    double xcothx_2 = c[0] + c[1] * xx + c[2] * (xx * xx);

    return 2 * x / (xcothx_2 - x);
}
#endif

/* vim: set ft=c: */
