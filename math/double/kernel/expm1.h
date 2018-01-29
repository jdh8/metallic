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

inline double __kernel_expm1(double x, double y)
{
    const double p[] = {
        1.6666666666666602132e-1,
       -2.7777777777015831021e-3,
        6.6137563215696832943e-5,
       -1.6533902379026328264e-6,
        4.1381437643920980023e-8
    };

    double r = x - y;
    double r2 = r * r;
    double r4 = r2 * r2;

    double c = r - (p[0] * r2 + (p[1] + p[2] * r2) * r4 + (p[3] + p[4] * r2) * (r4 * r4));

    return r * c / (2 - c) - y + x;
}

#endif
/* vim: set ft=c: */
