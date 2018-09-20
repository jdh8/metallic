/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#ifndef METALLIC_KERNEL_EXPM1_H
#define METALLIC_KERNEL_EXPM1_H

static double _complement_exp1(double x)
{
    const double p[] = {
        1.6666666666666602132e-1,
       -2.7777777777015831021e-3,
        6.6137563215696832943e-5,
       -1.6533902379026328264e-6,
        4.1381437643920980023e-8
    };

    double xx = x * x;

    return p[0] * x + (p[1] + p[2] * x) * xx + (p[3] + p[4] * x) * (xx * xx);
}

static double _kernel_expm1(double r)
{
    double c = r - _complement_exp1(r * r);

    return r * c / (2 - c);
}

static double _kernel_exp2(double x, double y)
{
    return _kernel_exp1(x - y) - y + x;
}

#endif
