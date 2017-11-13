/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
/*!
 * \brief Kernel of atanhf
 *
 * This computes inverse hyperbolic tangent in a domain of \f$ \left[
 * \frac{1 - \sqrt2}{1 + \sqrt2}, \frac{\sqrt2 - 1}{\sqrt2 + 1} \right] \f$.
 * The result is guaranteed to be faithfully rounded in
 * \c float, whose relative error is controlled within 6.927795e-10.
 *
 * If \a x is outside of \f$ \left[ \frac{1 - \sqrt2}{1 + \sqrt2},
 * \frac{\sqrt2 - 1}{\sqrt2 + 1} \right] \f$ the result is inaccurate.
 *
 * \param x - The argument in \f$ \left[ \frac{1 - \sqrt2}{1 + \sqrt2},
 * \frac{\sqrt2 - 1}{\sqrt2 + 1} \right] \f$ 
 * \return  Approximate \f$ \operatorname{atanh}(x) \f$ as precise as \c float.
 */
inline double __kernel_atanhf(double x)
{
    const double c[] = {
        0.9999999993072205474,
        0.3333340818599626478,
        0.1998737838945025914,
        0.1496325726858180278
    };

    double xx = x * x;

    return x * (c[0] + c[1] * xx + (c[2] + c[3] * xx) * (xx * xx));
}
