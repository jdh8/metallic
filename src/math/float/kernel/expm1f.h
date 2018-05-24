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
 * This computes exponential functon in a domain of
 * \f$ \left[ -\frac{\ln2}{2}, \frac{\ln2}{2} \right] \f$.
 * The result is guaranteed to be faithfully rounded in \c float, whose
 * relative error is controlled within 1.078561e-8.
 *
 * If \a x is outside of
 * \f$ \left[ -\frac{\ln2}{2}, \frac{\ln2}{2} \right] \f$,
 * the result is inaccurate.
 *
 * \param x - The argument in \f$ \left[ -\frac{\ln2}{2}, \frac{\ln2}{2} \right] \f$
 * \return  Approximate \f$ \exp x - 1 \f$ as precise as \c float.
 */
inline double __kernel_expm1f(double x)
{
    const double c[] = {
        1.000000010775500705,
        5.000000080819903627e-1,
        1.666650523422326531e-1,
        4.166624066361261157e-2,
        8.369150671031008566e-3,
        1.394858354331218335e-3
    };

    double xx = x * x;

    return x * (c[0] + c[1] * x + (c[2] + c[3] * x) * xx + (c[4] + c[5] * x) * (xx * xx));
}
#endif
/* vim: set ft=c: */
