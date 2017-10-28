/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
static double kernel_atanhf(double x)
{
    double xx = x * x;
    double num = 10.6182142303856372 - 5.82312460767683371 * xx;
    double den = 10.6182148483364447 + (xx - 9.36263404254862304) * xx;

    return x * num / den;
}
