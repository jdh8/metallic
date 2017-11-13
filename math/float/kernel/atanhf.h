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
 * \c float, whose relative error is controlled within 1.912297e-8.
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
    double xx = x * x;

    return x * (3.707126273856395376 - xx)
        / (3.707126202965120879 - 2.235666074879315124 * xx);
}
