/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#ifndef METALLIC_KERNEL_EXPF_H
#define METALLIC_KERNEL_EXPF_H

/* Restriction of `expm1f` to [-0.5 ln 2, 0.5 ln 2] */
static double _kernel_expf(double x)
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
