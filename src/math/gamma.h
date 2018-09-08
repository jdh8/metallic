/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
static double __gamma_lanczos_sum(double z)
{
    const double p[] = {
        0.99999999999980993227684700473478,
        676.520368121885098567009190444019,
       -1259.13921672240287047156078755283,
        771.3234287776530788486528258894,
       -176.61502916214059906584551354,
        12.507343278686904814458936853,
       -0.13857109526572011689554707,
        9.984369578019570859563e-6,
        1.50563273514931155834e-7
    };

    double sum = -0.0;

    for (int i = 8; i; --i)
        sum += p[i] / (i - 1 + z);

    return sum + p[0];
}
