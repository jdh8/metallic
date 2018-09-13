/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#ifndef METALLIC_KERNEL_EXP2F_H
#define METALLIC_KERNEL_EXP2F_H
/*!
 * \brief Kernel of exp2f
 *
 * This computes exponential functon in a domain of \f$ [-0.5,
 * 0.5] \f$.  The result is guaranteed to be faithfully rounded in
 * \c float, whose relative error is controlled within 7.476016e-9.
 *
 * If \a x is outside of \f$ [-0.5, 0.5] \f$,
 * the result is inaccurate.
 *
 * \param x - The argument in \f$ [-0.5, 0.5] \f$
 * \return  Approximate \f$ 2^x - 1 \f$ as precise as \c float.
 */
static double _kernel_exp2m1f(double x)
{
    const double c[] = {
        6.931471880289532425e-1,
        2.402265108421173406e-1,
        5.550357105498874537e-2,
        9.618030771171497658e-3,
        1.339086685300950937e-3,
        1.546973499989028719e-4
    };

    double xx = x * x;

    return x * (c[0] + c[1] * x + (c[2] + c[3] * x) * xx + (c[4] + c[5] * x) * (xx * xx));
}
#endif
