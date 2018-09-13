/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
/*!
 * \brief Kernel of atanf
 *
 * This function computes arctangent in a domain of \f$ [-1, 1] \f$.
 * The result is guaranteed to be faithfully rounded in \c float,
 * whose relative error is controlled within 1.796367e-08.
 *
 * \param x - The argument in \f$ [-1, 1] \f$
 * \return  Approximate \f$ \arctan x \f$ as precise as \c float.
 */
static double _kernel_atanf(double x)
{
    const double n[] = {
        80.3271869581482272,
        72.2722811622844352,
        11.0979939649070095
    };

    const double d[] = {
        80.3271884011195215,
        99.0478590867740213,
        28.0510393790794482,
    };

    double x2 = x * x;
    double x4 = x2 * x2;
    double num = n[0] + n[1] * x2 + n[2] * x4;
    double den = d[0] + d[1] * x2 + (d[2] + x2) * x4;

    return x * num / den;
}
