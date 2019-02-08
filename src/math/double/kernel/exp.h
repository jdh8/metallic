/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2019 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
static double _kernel_expa(double x)
{
    const double c[] = {
        1.6666666666666661553e-1,
       -2.7777777777564778614e-3,
        6.6137564717940089175e-5,
       -1.6534060280704224368e-6,
        4.1437773411068822733e-8
    };

    double xx = x * x;

    return c[0] * x + (c[1] + c[2] * x) * xx + (c[3] + c[4] * x) * (xx * xx);
}

static double _kernel_expb(double greater, double lesser)
{
    double x = greater + lesser;
    double y = x - _kernel_expa(x * x);

    return x * y / (2 - y) + lesser + greater + 1;
}
